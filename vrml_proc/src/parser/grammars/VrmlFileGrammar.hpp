#pragma once

#include <memory>
#include <string>

#define BOOST_SPIRIT_USE_PHOENIX_V3
#include <boost/phoenix/phoenix.hpp>

#include <boost/spirit/include/qi.hpp>
#include <boost/fusion/adapted/struct/adapt_struct.hpp>

#include <boost/optional.hpp>
#include <boost/variant/variant.hpp>
#include <boost/variant/recursive_wrapper.hpp>

#include "VrmlField.hpp"
#include "VrmlFile.hpp"
#include "VrmlNode.hpp"
#include "UseNode.hpp"
#include "VrmlNodeManager.hpp"

#include "IdentifierGrammar.hpp"
#include "Vec2fGrammar.hpp"
#include "Vec3fGrammar.hpp"
#include "Vec2fArrayGrammar.hpp"
#include "Vec3fArrayGrammar.hpp"
#include "Vec4fGrammar.hpp"
#include "Int32ArrayGrammar.hpp"
#include "QuotedStringGrammar.hpp"
#include "BooleanGrammar.hpp"
#include "BaseGrammar.hpp"

BOOST_FUSION_ADAPT_STRUCT(vrml_proc::parser::VrmlNode,
                          (boost::optional<std::string>,
                           definitionName)(std::string, header)(std::vector<vrml_proc::parser::VrmlField>, fields))

BOOST_FUSION_ADAPT_STRUCT(vrml_proc::parser::UseNode, (std::string, identifier))

BOOST_FUSION_ADAPT_STRUCT(vrml_proc::parser::VrmlField, (std::string, name)(vrml_proc::parser::VrmlFieldValue, value))

namespace vrml_proc {
  namespace parser {

    // For more information, see:
    // https://www.boost.org/doc/libs/1_87_0/libs/spirit/doc/html/spirit/qi/reference/numeric/real.html.
    struct Float32Policy : boost::spirit::qi::real_policies<float32_t> {
      static bool const expect_dot = true;
    };

    template <typename Iterator, typename Skipper>
    class VrmlFileGrammar : public boost::spirit::qi::grammar<Iterator, VrmlFile(), Skipper>,
                            public BaseGrammar<Iterator, VrmlFile(), Skipper> {
     public:
      VrmlFileGrammar() : VrmlFileGrammar::base_type(this->m_start) {
        m_identifier = std::make_unique<IdentifierGrammar<Iterator, Skipper>>();

        m_quotedString = std::make_unique<QuotedStringGrammar<Iterator, Skipper>>();

        m_vec2f = std::make_unique<Vec2fGrammar<Iterator, Skipper>>();

        m_vec3f = std::make_unique<Vec3fGrammar<Iterator, Skipper>>();

        m_vec4f = std::make_unique<Vec4fGrammar<Iterator, Skipper>>();

        m_vec2fArray = std::make_unique<Vec2fArrayGrammar<Iterator, Skipper>>();

        m_vec3fArray = std::make_unique<Vec3fArrayGrammar<Iterator, Skipper>>();

        m_int32Array = std::make_unique<Int32ArrayGrammar<Iterator, Skipper>>();

        m_boolean = std::make_unique<BooleanGrammar<Iterator, Skipper>>();

        m_vrmlFieldValue = (m_quotedString->GetStartRule() | m_boolean->GetStartRule() | m_vec3fArray->GetStartRule() |
                            m_vec2fArray->GetStartRule() | m_int32Array->GetStartRule() | m_vec4f->GetStartRule() |
                            m_vec3f->GetStartRule() | m_vec2f->GetStartRule() |
                            boost::spirit::qi::real_parser<float32_t, Float32Policy>() | boost::spirit::qi::int_ |
                            m_useNode | m_vrmlNode | m_vrmlNodeArray);

        m_vrmlField = (m_identifier->GetStartRule() >> m_vrmlFieldValue);

        m_vrmlNode =
            (-(boost::spirit::qi::lit("DEF") >> m_identifier->GetStartRule()) >> m_identifier->GetStartRule() >>
             boost::spirit::qi::lit("{") >> *(m_vrmlField) >> boost::spirit::qi::lit("}"));

        m_vrmlNodeArray = "[" >> ((m_vrmlNode | m_useNode) % ",") >> "]";

        m_useNode = boost::spirit::qi::lit("USE") >> m_identifier->GetStartRule();

        this->m_start =
            boost::spirit::qi::skip(boost::spirit::ascii::space)[boost::spirit::qi::lit("#VRML V2.0 utf8")] >>
            *(m_vrmlNode);

        BOOST_SPIRIT_DEBUG_NODE(this->m_start);
        BOOST_SPIRIT_DEBUG_NODE(m_vrmlFieldValue);
        BOOST_SPIRIT_DEBUG_NODE(m_vrmlField);
        BOOST_SPIRIT_DEBUG_NODE(m_vrmlNode);
      }

     private:
      boost::spirit::qi::rule<Iterator, VrmlNode(), Skipper> m_vrmlNode;
      boost::spirit::qi::rule<Iterator, UseNode(), Skipper> m_useNode;
      boost::spirit::qi::rule<Iterator, VrmlField(), Skipper> m_vrmlField;
      boost::spirit::qi::rule<Iterator, VrmlFieldValue(), Skipper> m_vrmlFieldValue;
      boost::spirit::qi::rule<
          Iterator,
          std::vector<boost::variant<boost::recursive_wrapper<VrmlNode>, boost::recursive_wrapper<UseNode>>>(), Skipper>
          m_vrmlNodeArray;

      std::unique_ptr<IdentifierGrammar<Iterator, Skipper>> m_identifier;
      std::unique_ptr<Vec2fGrammar<Iterator, Skipper>> m_vec2f;
      std::unique_ptr<Vec3fGrammar<Iterator, Skipper>> m_vec3f;
      std::unique_ptr<Vec4fGrammar<Iterator, Skipper>> m_vec4f;
      std::unique_ptr<Vec2fArrayGrammar<Iterator, Skipper>> m_vec2fArray;
      std::unique_ptr<Vec3fArrayGrammar<Iterator, Skipper>> m_vec3fArray;
      std::unique_ptr<Int32ArrayGrammar<Iterator, Skipper>> m_int32Array;
      std::unique_ptr<QuotedStringGrammar<Iterator, Skipper>> m_quotedString;
      std::unique_ptr<BooleanGrammar<Iterator, Skipper>> m_boolean;
    };
  }  // namespace parser
}  // namespace vrml_proc
