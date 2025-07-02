#pragma once

#include <boost/spirit/include/phoenix.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/qi_int.hpp>

#include "BaseGrammar.hpp"

namespace vrml_proc::parser::grammar {
  /**
   * @brief Grammar for parsing SF type of 32-bit integer value in VRML 2.0 syntax.
   *
   * @tparam Iterator iterator type used for parsing input
   * @tparam Skipper  skipper parser used to skip irrelevant input (e.g., whitespace)
   */
  template <typename Iterator, typename Skipper>
  struct Int32Grammar : public boost::spirit::qi::grammar<Iterator, int32_t(), Skipper>,
                        public BaseGrammar<Iterator, int32_t(), Skipper> {
   public:
    /**
     * @brief Constructs new grammar and initializes parsing rules.
     */
    Int32Grammar() : Int32Grammar::base_type(this->m_start) {  //

      namespace ascii = boost::spirit::ascii;
      namespace qi = boost::spirit::qi;
      namespace phx = boost::phoenix;

      boost::spirit::qi::int_parser<int32_t, 16, 1, 8> hexParser;

      hexRule = qi::lexeme[(qi::lit("0x") >> hexParser)[qi::_val = qi::_1] |
                           (qi::lit("-0x") >> hexParser)[qi::_val = -qi::_1]];

      this->m_start = hexRule | qi::int_;

      BOOST_SPIRIT_DEBUG_NODE(this->m_start);
      BOOST_SPIRIT_DEBUG_NODE(hexRule);
    }

   private:
    boost::spirit::qi::rule<Iterator, int32_t(), Skipper> hexRule;
  };
}  // namespace vrml_proc::parser::grammar