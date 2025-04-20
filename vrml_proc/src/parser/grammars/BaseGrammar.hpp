#pragma once

#include <boost/spirit/include/qi.hpp>

namespace vrml_proc::parser {
  /**
   * @brief Represents a base grammar.
   * Each derived grammar has to implement ` GetStartRule()` method. Base grammar should also store
   * information about an error encountered in any grammara while parsing.
   *
   * @tparam Iterator type of iteraror used, e.g. const char* iterator
   * @tparam ReturnType type of result grammar will return
   * @tparam Skipper type of skipper which is a rule or grammar of the same `Iterator` type which defines what
   * characters should be skipped while parsing
   */
  template <typename Iterator, typename ReturnType, typename Skipper>
  class BaseGrammar {
   public:
    virtual ~BaseGrammar() = default;
    boost::spirit::qi::rule<Iterator, ReturnType, Skipper> const& GetStartRule() const { return m_start; }

   protected:
    BaseGrammar() = default;
    boost::spirit::qi::rule<Iterator, ReturnType, Skipper> m_start;
    bool HasError() const { return m_hasError; }
    void SetError(bool value) { m_hasError = value; }

   private:
    bool m_hasError = false;
    BaseGrammar(const BaseGrammar&) = delete;
    BaseGrammar& operator=(const BaseGrammar&) = delete;
    BaseGrammar(BaseGrammar&&) = delete;
    BaseGrammar& operator=(BaseGrammar&&) = delete;
  };
}  // namespace vrml_proc::parser
