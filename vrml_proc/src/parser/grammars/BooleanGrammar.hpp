#pragma once

#include <boost/spirit/include/qi.hpp>
#include "BaseGrammar.hpp"

namespace vrml_proc::parser {
  template <typename Iterator, typename Skipper>
  class BooleanGrammar : public boost::spirit::qi::grammar<Iterator, bool(), Skipper>,
                         public BaseGrammar<Iterator, bool(), Skipper> {
   public:
    BooleanGrammar() : BooleanGrammar::base_type(this->m_start) {
      this->m_start = (boost::spirit::qi::lit("TRUE")[boost::spirit::qi::_val = true] |
                       boost::spirit::qi::lit("FALSE")[boost::spirit::qi::_val = false]);

      BOOST_SPIRIT_DEBUG_NODE(this->m_start);
    }
  };
}  // namespace vrml_proc::parser
