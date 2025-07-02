#pragma once

#include <memory>

#include <boost/spirit/include/qi.hpp>
#include <boost/fusion/adapted/struct/adapt_struct.hpp>

#include "Vec2f.hpp"
#include "Vec2fArray.hpp"
#include "Vec2fGrammar.hpp"
#include "BaseGrammar.hpp"

BOOST_FUSION_ADAPT_STRUCT(vrml_proc::parser::model::Vec2fArray, (std::vector<vrml_proc::parser::model::Vec2f>, vectors))

namespace vrml_proc::parser::grammar {
  /**
   * @brief Grammar for parsing MF type of Vec2 values in VRML 2.0 syntax.
   *
   * @tparam Iterator The iterator type used for parsing input.
   * @tparam Skipper  The skipper parser used to skip irrelevant input (e.g., whitespace).
   */
  template <typename Iterator, typename Skipper>
  class Vec2fArrayGrammar : public boost::spirit::qi::grammar<Iterator, model::Vec2fArray(), Skipper>,
                            public BaseGrammar<Iterator, model::Vec2fArray(), Skipper> {
   public:
    /**
     * @brief Constructs new grammar and initializes parsing rules.
     */
    Vec2fArrayGrammar() : Vec2fArrayGrammar::base_type(this->m_start), m_vec2f() {
      this->m_start = boost::spirit::qi::lit('[') >>
                      -((m_vec2f.GetStartRule() % boost::spirit::qi::lit(',')) >> -boost::spirit::qi::lit(',')) >>
                      boost::spirit::qi::lit(']');

      BOOST_SPIRIT_DEBUG_NODE(this->m_start);
    }

   private:
    Vec2fGrammar<Iterator, Skipper> m_vec2f;
  };
}  // namespace vrml_proc::parser::grammar
