#pragma once
#include <string>

#include <boost/spirit/include/qi.hpp>
#include "BaseGrammar.hpp"

namespace vrml_proc::parser {
  /**
   * @brief Represents a grammar for parsing identifer as defined in VRML 2.0 specification.
   *
   * @tparam Iterator type of iteraror used, e.g. const char*
   * @tparam Skipper type of skipper which is a rule or grammar of the same `Iterator` type which defines which
   * characters should be skipped while parsing
   */
  template <typename Iterator, typename Skipper>
  class IdentifierGrammar : public boost::spirit::qi::grammar<Iterator, std::string(), Skipper>,
                            public BaseGrammar<Iterator, std::string(), Skipper> {
   public:
    IdentifierGrammar() : IdentifierGrammar::base_type(this->m_start) {  //

      using boost::spirit::qi::char_;
      using boost::spirit::qi::lexeme;

      this->m_start = lexeme[+(char_ - (char_(0x00, 0x20) | char_(0x30, 0x39) | char_("\"'+,-.[]{}\\"))) >>
                             *(char_ - (char_(0x00, 0x20) | char_("\"'+,-.[]{}\\")))];

      BOOST_SPIRIT_DEBUG_NODE(this->m_start);
    }
  };
}  // namespace vrml_proc::parser
