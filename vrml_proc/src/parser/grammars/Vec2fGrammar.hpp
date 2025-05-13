#pragma once

#include <iostream>
#include <iterator>
#include <string>

#include <boost/fusion/adapted/struct/adapt_struct.hpp>
#include <boost/phoenix/bind/bind_member_function.hpp>
#include <boost/phoenix/object/construct.hpp>
#include <boost/phoenix/operator.hpp>
#include <boost/spirit/include/qi.hpp>

#include "BaseGrammar.hpp"
#include "Vec2f.hpp"

BOOST_FUSION_ADAPT_STRUCT(vrml_proc::parser::Vec2f, (vrml_proc::parser::float32_t, u)(vrml_proc::parser::float32_t, v))

namespace vrml_proc::parser::grammar {
  /**
   * @brief Grammar for parsing SF type of Vec2f value in VRML 2.0 syntax.
   *
   * @tparam Iterator The iterator type used for parsing input.
   * @tparam Skipper  The skipper parser used to skip irrelevant input (e.g., whitespace).
   */
  template <typename Iterator, typename Skipper>
  struct Vec2fGrammar : public boost::spirit::qi::grammar<Iterator, Vec2f(), Skipper>,
                        public BaseGrammar<Iterator, Vec2f(), Skipper> {
   public:
    /**
     * @brief Constructs new grammar and initializes parsing rules.
     */
    Vec2fGrammar() : Vec2fGrammar::base_type(this->m_start) {
      float32 = boost::spirit::qi::float_;
      this->m_start = float32 >> float32;  // '>' will trigger error, '>>' won't

      BOOST_SPIRIT_DEBUG_NODE(this->m_start);
      BOOST_SPIRIT_DEBUG_NODE(float32);

      /**
       * @brief Currently unused proposal of error-catching mechanism in grammars.
       *
       * _1 is Iterator at the beginning of the string
       * _2 is Iterator of the end
       * _3 is Iterator of the error position
       * _4 is info object containing expected rule
       */
      /*boost::spirit::qi::on_error<boost::spirit::qi::fail>(
          this->m_start,
          std::cout << boost::phoenix::val("Error! Expecting: ")
          << boost::spirit::qi::_4
          << boost::phoenix::val(". Error position")
          << boost::phoenix::bind(&Vec2fGrammar::ConstructErrorPositionString, this, boost::spirit::qi::_1,
      boost::spirit::qi::_3, boost::spirit::qi::_2, boost::spirit::qi::_4, 16)
          << std::endl
      );*/
    }

    /**
     * @brief Constructs an error position message.
     *
     * @note Currently unused proposal of error-catching mechanism in grammars.
     */
    std::string ConstructErrorPositionString(const Iterator& begin,
        const Iterator& errorPosition,
        const Iterator& end,
        const boost::spirit::info& expectedRule,
        size_t maxLength) {  //

      /**
       * 1) distance from begin to errorPosition
       * 2) distance from errorPosition to end
       * 3) store expectedRule as string
       * 4) create (truncated) errorPosition as string
       * 5) store this into std::optional<vrml_proc::parser::error::GrammarError> variable as this->m_error = ...
       * 6) BaseGrammar::HasError() const will check m_eroro variable and return boolean according to it
       */

      this->SetError(true);

      if (errorPosition == end) {
        return " is at the end of the input string.";
      }
      size_t length = std::distance(errorPosition, end);

      if (length > maxLength) {
        return ": '" + std::string(errorPosition, errorPosition + maxLength) + "...'";
      }
      return "'" + std::string(errorPosition, end) + "'";
    }

   private:
    boost::spirit::qi::rule<Iterator, float, Skipper> float32;
  };
}  // namespace vrml_proc::parser::grammar
