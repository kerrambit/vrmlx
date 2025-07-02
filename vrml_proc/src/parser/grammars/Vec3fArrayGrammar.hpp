#pragma once

#include <boost/fusion/adapted/struct/adapt_struct.hpp>
#include <boost/spirit/include/qi.hpp>

#include "BaseGrammar.hpp"
#include "Vec3f.hpp"
#include "Vec3fArray.hpp"
#include "Vec3fGrammar.hpp"

BOOST_FUSION_ADAPT_STRUCT(vrml_proc::parser::model::Vec3fArray, (std::vector<vrml_proc::parser::model::Vec3f>, vectors))

namespace vrml_proc::parser::grammar {
  /**
   * @brief Grammar for parsing MF type of Vec3f values in VRML 2.0 syntax.
   *
   * @tparam Iterator The iterator type used for parsing input.
   * @tparam Skipper  The skipper parser used to skip irrelevant input (e.g., whitespace).
   */
  template <typename Iterator, typename Skipper>
  class Vec3fArrayGrammar : public boost::spirit::qi::grammar<Iterator, model::Vec3fArray(), Skipper>,
                            public BaseGrammar<Iterator, model::Vec3fArray(), Skipper> {
   public:
    /**
     * @brief Constructs new grammar and initializes parsing rules.
     */
    Vec3fArrayGrammar() : Vec3fArrayGrammar::base_type(this->m_start), m_vec3f() {  //

      using boost::spirit::qi::lit;

      const auto comma = lit(',');
      const auto openBrace = lit("[");
      const auto closeBrace = lit("]");

      this->m_start = openBrace >> -((m_vec3f.GetStartRule() % comma) >> -comma) >> closeBrace;

      BOOST_SPIRIT_DEBUG_NODE(this->m_start);
    }

   private:
    Vec3fGrammar<Iterator, Skipper> m_vec3f;
  };
}  // namespace vrml_proc::parser::grammar
