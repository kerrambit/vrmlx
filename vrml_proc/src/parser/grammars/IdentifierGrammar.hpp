#pragma once
#include <string>

#include <boost/spirit/include/qi.hpp>
#include "BaseGrammar.hpp"

namespace vrml_proc::parser {
  /**
   * @brief Represents a grammar for parsign identifer as defined in VRML 2.0 specification.
   */
  template <typename Iterator, typename Skipper>
  class IdentifierGrammar : public boost::spirit::qi::grammar<Iterator, std::string(), Skipper>,
                            public BaseGrammar<Iterator, std::string(), Skipper> {
   public:
    IdentifierGrammar() : IdentifierGrammar::base_type(this->m_start) {
      this->m_start =
          boost::spirit::qi::lexeme[+(boost::spirit::qi::char_ - (boost::spirit::qi::char_(0x00, 0x20) |
                                                                     boost::spirit::qi::char_(0x30, 0x39) |
                                                                     boost::spirit::qi::char_("\"'+,-.[]{}\\"))) >>
                                    *(boost::spirit::qi::char_ - (boost::spirit::qi::char_(0x00, 0x20) |
                                                                     boost::spirit::qi::char_("\"'+,-.[]{}\\")))];

      BOOST_SPIRIT_DEBUG_NODE(this->m_start);
    }
  };
}  // namespace vrml_proc::parser
