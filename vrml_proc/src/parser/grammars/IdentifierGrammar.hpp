#pragma once
#include <string>

#include <boost/spirit/include/qi.hpp>
#include "BaseGrammar.hpp"

namespace vrml_proc::parser::grammar {
  /**
   * @brief Represents a grammar for parsing identifer as defined in VRML 2.0 specification.
   *
   * @tparam Iterator The iterator type used for parsing input.
   * @tparam Skipper  The skipper parser used to skip irrelevant input (e.g., whitespace).
   */
  template <typename Iterator, typename Skipper>
  class IdentifierGrammar : public boost::spirit::qi::grammar<Iterator, std::string(), Skipper>,
                            public BaseGrammar<Iterator, std::string(), Skipper> {
   public:
    /**
     * @brief Constructs the identifier grammar and initializes parsing rules.
     */
    IdentifierGrammar() : IdentifierGrammar::base_type(this->m_start) {  //

      using boost::spirit::qi::char_;
      using boost::spirit::qi::lexeme;

      this->m_start = lexeme[+(char_ - (char_(0x00, 0x20) | char_(0x30, 0x39) | char_("\"'+,-.[]{}\\"))) >>
                             *(char_ - (char_(0x00, 0x20) | char_("\"'+,-.[]{}\\")))];

      BOOST_SPIRIT_DEBUG_NODE(this->m_start);
    }
  };
}  // namespace vrml_proc::parser::grammar
