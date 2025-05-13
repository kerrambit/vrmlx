#pragma once

// #define BOOST_SPIRIT_DEBUG

#include "BufferView.hpp"
#include "CommentSkipper.hpp"
#include "Parser.hpp"
#include "VrmlFile.hpp"
#include "VrmlFileGrammar.hpp"
#include "MemoryMappedFileReader.hpp"

#include "VrmlProcExport.hpp"

namespace vrml_proc::parser {
  /**
   * @brief Represents a VRML 2.0 parser. It takes const char* as input, respectivally their begin and end pointers.
   *
   * Note that not the whole standart is supported.
   */
  class VRMLPROC_API VrmlParser : public Parser<BufferView, VrmlFile> {
   public:
    VrmlParser() = delete;

    /**
     * @brief Constructs new parser.
     *
     * @param manager reference to VrmlNodemanager which will be populated with data in the source of parsing
     */
    VrmlParser(service::VrmlNodeManager& manager)
        : Parser<BufferView, VrmlFile>(), m_manager(manager), m_grammar(), m_skipper() {}

    /**
     * @brief Parses the VRML 2.0 file.
     *
     * @param buffer object containing const char pointers indicating begin and end for const char* to parse.
     * @returns vector of VRML nodes aka VRML file if parsing is succefull, otherwise error
     */
    ParserResult<VrmlFile> Parse(BufferView buffer) override;

   private:
    grammar::VrmlFileGrammar<const char*, grammar::CommentSkipper> m_grammar;
    grammar::CommentSkipper m_skipper;
    service::VrmlNodeManager& m_manager;
  };
}  // namespace vrml_proc::parser
