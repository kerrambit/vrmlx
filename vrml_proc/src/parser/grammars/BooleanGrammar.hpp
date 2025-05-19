#pragma once

#include <boost/spirit/include/qi.hpp>

#include "BaseGrammar.hpp"

namespace vrml_proc::parser::grammar {
  /**
   * @brief Grammar for parsing boolean values (`TRUE` or `FALSE`) in VRML 2.0 syntax.
   *
   * @tparam Iterator The iterator type used for parsing input.
   * @tparam Skipper  The skipper parser used to skip irrelevant input (e.g., whitespace).
   */
  template <typename Iterator, typename Skipper>
  class BooleanGrammar : public boost::spirit::qi::grammar<Iterator, bool(), Skipper>,
                         public BaseGrammar<Iterator, bool(), Skipper> {
   public:
    /**
     * @brief Constructs the boolean grammar and initializes parsing rules.
     */
    BooleanGrammar() : BooleanGrammar::base_type(this->m_start) {
      this->m_start = (boost::spirit::qi::lit("TRUE")[boost::spirit::qi::_val = true] |
                       boost::spirit::qi::lit("FALSE")[boost::spirit::qi::_val = false]);

      BOOST_SPIRIT_DEBUG_NODE(this->m_start);
    }
  };
}  // namespace vrml_proc::parser::grammar
