#pragma once

#include <string>

#include <boost/spirit/include/qi.hpp>

#include "BaseGrammar.hpp"

namespace vrml_proc::parser::grammar {
  /**
   * @brief Grammar for parsing SF type of string in VRML 2.0 syntax.
   *
   * @tparam Iterator The iterator type used for parsing input.
   * @tparam Skipper  The skipper parser used to skip irrelevant input (e.g., whitespace).
   */
  template <typename Iterator, typename Skipper>
  class QuotedStringGrammar : public boost::spirit::qi::grammar<Iterator, std::string(), Skipper>,
                              public BaseGrammar<Iterator, std::string(), Skipper> {
   public:
    /**
     * @brief Constructs new grammar and initializes parsing rules.
     */
    QuotedStringGrammar() : QuotedStringGrammar::base_type(this->m_start) {
      this->m_start = boost::spirit::qi::lexeme['"' >> +(boost::spirit::qi::char_ - '"') >> '"'];

      BOOST_SPIRIT_DEBUG_NODE(this->m_start);
    }
  };
}  // namespace vrml_proc::parser::grammar
