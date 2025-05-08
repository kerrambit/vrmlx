#pragma once

#include <boost/fusion/adapted/struct/adapt_struct.hpp>
#include <boost/phoenix/bind/bind_member_function.hpp>
#include <boost/phoenix/object/construct.hpp>
#include <boost/phoenix/operator.hpp>
#include <boost/spirit/include/qi.hpp>

#include "BaseGrammar.hpp"
#include "Vec3f.hpp"

BOOST_FUSION_ADAPT_STRUCT(vrml_proc::parser::Vec3f,
    (vrml_proc::parser::float32_t, x)(vrml_proc::parser::float32_t, y)(vrml_proc::parser::float32_t, z))

namespace vrml_proc::parser {
  /**
   * @brief Grammar for parsing SF type of Vec3f value in VRML 2.0 syntax.
   *
   * @tparam Iterator The iterator type used for parsing input.
   * @tparam Skipper  The skipper parser used to skip irrelevant input (e.g., whitespace).
   */
  template <typename Iterator, typename Skipper>
  struct Vec3fGrammar : public boost::spirit::qi::grammar<Iterator, Vec3f(), Skipper>,
                        public BaseGrammar<Iterator, Vec3f(), Skipper> {
   public:
    /**
     * @brief Constructs new grammar and initializes parsing rules.
     */
    Vec3fGrammar() : Vec3fGrammar::base_type(this->m_start) {
      float32 = boost::spirit::qi::float_;
      this->m_start = float32 >> float32 >> float32;

      BOOST_SPIRIT_DEBUG_NODE(this->m_start);
      BOOST_SPIRIT_DEBUG_NODE(float32);
    }

   private:
    boost::spirit::qi::rule<Iterator, float, Skipper> float32;
  };
}  // namespace vrml_proc::parser
