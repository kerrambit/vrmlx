#pragma once

#include <boost/fusion/adapted/struct/adapt_struct.hpp>
#include <boost/spirit/include/qi.hpp>

#include "BaseGrammar.hpp"
#include "Int32Array.hpp"
#include "Int32Grammar.hpp"

BOOST_FUSION_ADAPT_STRUCT(vrml_proc::parser::model::Int32Array, (std::vector<int32_t>, integers))

namespace vrml_proc::parser::grammar {
  /**
   * @brief Grammar for parsing MF type of Int32 values in VRML 2.0 syntax.
   *
   * @tparam Iterator The iterator type used for parsing input.
   * @tparam Skipper  The skipper parser used to skip irrelevant input (e.g., whitespace).
   */
  template <typename Iterator, typename Skipper>
  class Int32ArrayGrammar : public boost::spirit::qi::grammar<Iterator, model::Int32Array(), Skipper>,
                            public BaseGrammar<Iterator, model::Int32Array(), Skipper> {
   public:
    /**
     * @brief Constructs new grammar and initializes parsing rules.
     */
    Int32ArrayGrammar() : Int32ArrayGrammar::base_type(this->m_start), m_int32() {
      this->m_start = "[" >> (m_int32.GetStartRule() % ",") >> -boost::spirit::qi::lit(',') >> "]";

      BOOST_SPIRIT_DEBUG_NODE(this->m_start);
    }

   private:
    Int32Grammar<Iterator, Skipper> m_int32;
  };
}  // namespace vrml_proc::parser::grammar
