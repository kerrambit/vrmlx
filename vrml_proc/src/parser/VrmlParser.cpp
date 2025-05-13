#include "VrmlParser.hpp"

#include <boost/spirit/home/qi/parse.hpp>

#include "BufferView.hpp"
#include "FormatString.hpp"
#include "Logger.hpp"
#include "ParserError.hpp"
#include "ParserResult.hpp"
#include "ScopedTimer.hpp"
#include "VrmlFile.hpp"
#include "VrmlNodeManagerPopulator.hpp"

namespace vrml_proc::parser {
  ParserResult<model::VrmlFile> VrmlParser::Parse(BufferView buffer) {  //

    using namespace vrml_proc::core::logger;
    using namespace vrml_proc::core::utils;

    LogInfo("Parse VRML file content.", LOGGING_INFO);

    model::VrmlFile parsedData;
    double time = 0.0;
    bool success = false;
    {
      auto timer = ScopedTimer(time);
      success = boost::spirit::qi::phrase_parse(buffer.begin, buffer.end, m_grammar, m_skipper, parsedData);
    }

    if (success && buffer.begin == buffer.end) {
      LogInfo(
          FormatString("Parsing was successful. The whole parsing and AST creation process took ", time, " seconds."),
          LOGGING_INFO);

      LogInfo("Populate VrmlNodeManager with DEF nodes.", LOGGING_INFO);
      double time = 0.0;
      {
        auto timer = ScopedTimer(time);
        for (const auto& root : parsedData) {
          service::VrmlNodeManagerPopulator::Populate(m_manager, root);
        }
      }
      LogInfo(
          FormatString("DEF nodes populating has finished. The whole process took ", time, " seconds."), LOGGING_INFO);

      return parsedData;
    }

    LogInfo(FormatString("Parsing was not successful. The process took ", time, " seconds."), LOGGING_INFO);
    return cpp::fail(std::make_shared<vrml_proc::parser::error::ParserError>());
  }
}  // namespace vrml_proc::parser
