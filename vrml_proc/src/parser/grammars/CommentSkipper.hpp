#pragma once

#include <string>

#include <boost/spirit/include/qi.hpp>

namespace vrml_proc::parser {
  /**
   * @brief Skipper grammar with const char* Iterator aimed to skip whitespaces and VRML comments (line starting with
   * '#').
   */
  struct CommentSkipper : boost::spirit::qi::grammar<const char*> {
    CommentSkipper() : CommentSkipper::base_type(start) {
      /**
       * @brief Inspired by
       * https://stackoverflow.com/questions/44529864/how-to-skip-line-block-nested-block-comments-in-boost-spirit..
       */
      singleLineComment = '#' >> *(boost::spirit::qi::char_ - boost::spirit::qi::eol) >>
                          (boost::spirit::qi::eol | boost::spirit::qi::eoi);
      start = boost::spirit::ascii::space | singleLineComment;

      BOOST_SPIRIT_DEBUG_NODE(singleLineComment);
      BOOST_SPIRIT_DEBUG_NODE(start);
    }

    boost::spirit::qi::rule<const char*> singleLineComment;
    boost::spirit::qi::rule<const char*> start;
  };
}  // namespace vrml_proc::parser
