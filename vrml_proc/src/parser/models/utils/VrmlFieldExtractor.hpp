#pragma once

#include <functional>
#include <optional>
#include <string>
#include <type_traits>
#include <typeinfo>
#include <vector>

#include <boost/variant/recursive_wrapper.hpp>
#include <boost/variant/variant.hpp>

#include <result.hpp>

#include "ExtractorCache.hpp"
#include "FormatString.hpp"
#include "Logger.hpp"
#include "TypeToString.hpp"
#include "Vec2f.hpp"
#include "Vec2fArray.hpp"
#include "VrmlField.hpp"
#include "VrmlNodeManager.hpp"

// Forward declarations.
template <typename T>
struct ExtractorVisitor;

template <typename T>
struct VariantVisitor;

namespace vrml_proc::parser::model::utils::VrmlFieldExtractor {

  inline bool IsNamePresent(const std::string& name, const std::vector<vrml_proc::parser::VrmlField>& fields) {
    for (const auto& field : fields) {
      if (field.name == name) {
        return true;
      }
    }
    return false;
  }

  // --------------------------------------------------------

  enum class ExtractByNameError { FieldNotFound, ValidationError };

  // --------------------------------------------------------

  template <typename T>
  inline std::optional<std::reference_wrapper<const T>> ExtractExtended(
      const vrml_proc::parser::VrmlFieldValue& fieldValue, std::string& invalidType) {
    ExtractorVisitor<T> visitor;
    auto result = boost::apply_visitor(visitor, fieldValue);

    if (result.has_value()) {
      return result.value();

    } else {
      if (result.error().has_value()) {
        invalidType = result.error().value();
      }
      return {};
    }
  }

  template <typename T>
  inline std::optional<std::reference_wrapper<const T>> Extract(const vrml_proc::parser::VrmlFieldValue& fieldValue) {
    std::string invalidType;
    return vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractExtended<T>(fieldValue, invalidType);
  }

  // --------------------------------------------------------

  template <typename T>
  inline cpp::result<std::reference_wrapper<const T>, ExtractByNameError> ExtractByNameExtended(
      const std::string& name, const std::vector<vrml_proc::parser::VrmlField>& fields, std::string& invalidType) {
    for (const auto& field : fields) {
      if (field.name == name) {
        ExtractorVisitor<T> visitor;
        auto result = boost::apply_visitor(visitor, field.value);

        if (result.has_value()) {
          return result.value();

        } else {
          if (result.error().has_value()) {
            invalidType = result.error().value();
          }
          return cpp::fail(ExtractByNameError::ValidationError);
        }
      }
    }
    return cpp::fail(ExtractByNameError::FieldNotFound);
  }

  template <typename T>
  inline cpp::result<std::reference_wrapper<const T>, ExtractByNameError> ExtractByName(
      const std::string& name, const std::vector<vrml_proc::parser::VrmlField>& fields) {
    std::string invalidType;
    return ExtractByNameExtended<T>(name, fields, invalidType);
  }

  // --------------------------------------------------------

  enum class ExtractVrmlNodeError { FieldNotFound, ValidationError, UnknownUseNode };

  inline cpp::result<std::reference_wrapper<const vrml_proc::parser::VrmlNode>, ExtractVrmlNodeError>
  ExtractVrmlNodeExtended(const std::string& name,
      const std::vector<vrml_proc::parser::VrmlField>& fields,
      const vrml_proc::parser::VrmlNodeManager& manager,
      std::string& invalidType,
      std::string& useId) {
    if (IsNamePresent(name, fields)) {
      std::string error = "";
      auto vrmlNode = ExtractByNameExtended<vrml_proc::parser::VrmlNode>(name, fields, error);
      if (vrmlNode.has_value()) {
        return vrmlNode.value();
      }

      auto useNode = ExtractByName<vrml_proc::parser::UseNode>(name, fields);
      if (useNode.has_value()) {
        auto managerFound = manager.GetDefinitionNode(useNode.value().get().identifier);
        if (managerFound.has_value()) {
          return managerFound.value();
        } else {
          useId = useNode.value().get().identifier;
          return cpp::fail(ExtractVrmlNodeError::UnknownUseNode);
        }
      }

      invalidType = error;
      return cpp::fail(ExtractVrmlNodeError::ValidationError);
    } else {
      return cpp::fail(ExtractVrmlNodeError::FieldNotFound);
    }
  }

  inline cpp::result<std::reference_wrapper<const vrml_proc::parser::VrmlNode>, ExtractVrmlNodeError> ExtractVrmlNode(
      const std::string& name,
      const std::vector<vrml_proc::parser::VrmlField>& fields,
      const vrml_proc::parser::VrmlNodeManager& manager) {
    std::string invalidType;
    std::string useId;
    return ExtractVrmlNodeExtended(name, fields, manager, invalidType, useId);
  }

  // --------------------------------------------------------

  template <typename T>
  inline std::optional<std::reference_wrapper<const T>> ExtractFromVariantExtended(
      const boost::variant<boost::recursive_wrapper<vrml_proc::parser::VrmlNode>,
          boost::recursive_wrapper<vrml_proc::parser::UseNode>>& variant,
      std::string& invalidType) {
    VariantVisitor<T> visitor;
    auto result = boost::apply_visitor(visitor, variant);
    if (result.has_value()) {
      return result.value();
    }

    if (result.error().has_value()) {
      invalidType = result.error().value();
    }

    return {};
  }

  template <typename T>
  inline std::optional<std::reference_wrapper<const T>> ExtractFromVariant(
      const boost::variant<boost::recursive_wrapper<vrml_proc::parser::VrmlNode>,
          boost::recursive_wrapper<vrml_proc::parser::UseNode>>& variant) {
    std::string invalidType;
    return ExtractFromVariantExtended<T>(variant, invalidType);
  }

  // --------------------------------------------------------

  enum class ExtractVrmlNodeFromVariantError { ValidationError, UnknownUseNode };

  inline cpp::result<std::reference_wrapper<const vrml_proc::parser::VrmlNode>, ExtractVrmlNodeFromVariantError>
  ExtractVrmlNodeFromVariantExtended(const boost::variant<boost::recursive_wrapper<vrml_proc::parser::VrmlNode>,
                                         boost::recursive_wrapper<vrml_proc::parser::UseNode>>& variant,
      const vrml_proc::parser::VrmlNodeManager& manager,
      std::string& invalidType,
      std::string& useId) {
    std::string out;
    auto result = ExtractFromVariantExtended<vrml_proc::parser::VrmlNode>(variant, out);
    if (result.has_value()) {
      return result.value();
    }

    auto useNode = ExtractFromVariantExtended<vrml_proc::parser::UseNode>(variant, out);
    if (useNode.has_value()) {
      auto managerFound = manager.GetDefinitionNode(useNode.value().get().identifier);
      if (managerFound.has_value()) {
        return managerFound.value();
      } else {
        useId = useNode.value().get().identifier;
        return cpp::fail(ExtractVrmlNodeFromVariantError::UnknownUseNode);
      }
    }

    invalidType = out;
    return cpp::fail(ExtractVrmlNodeFromVariantError::ValidationError);
  }

  inline cpp::result<std::reference_wrapper<const vrml_proc::parser::VrmlNode>, ExtractVrmlNodeFromVariantError>
  ExtractVrmlNodeFromVariant(const boost::variant<boost::recursive_wrapper<vrml_proc::parser::VrmlNode>,
                                 boost::recursive_wrapper<vrml_proc::parser::UseNode>>& variant,
      const vrml_proc::parser::VrmlNodeManager& manager) {
    std::string invalidType;
    std::string useId;
    return ExtractVrmlNodeFromVariantExtended(variant, manager, invalidType, useId);
  }
}  // namespace vrml_proc::parser::model::utils::VrmlFieldExtractor

/**
 * @brief Visitor for VRML field value extraction.
 *
 * @returns result type where value is const reference to queried data entry; if there is error, string describing
 * unexpected error type is returned (or possibly other error message)
 *
 * @note The visitor should be thread-safe. It uses only read-only data. There are two exceptions to this. 1. case is
 * for Vec3fArray, where it is needed to examine, if the value is not empty array, then we check other possible types
 * <T> if they are not equal. Possibly we create empty static array in such cases only initialized once (guaranteed by
 * C++11+ thread-safe static initialization). So it should be safe. 2. case is about float/int situation. The grammar
 * handles all (singular!) number without decimal points as ints. The following table shows the behavirour ('X' signs
 * where the special treatment has to be made to be able to e.g. take value '0' as both int and float):
 *
 * number 0 (as int)
 * number 0 (as float) X
 * number 5.0 (as int) X
 * number 5.0 (as float)
 * number 5.5 (as float)
 *
 * The logic is that we convert int to float and then store this value inside cache (see ExtractorCache class) or we try
 * to convert float to int (must be in format xyz.0) and also store it. If we encounter the query for the problematic
 * value, we look it up inside the map to retrieve the same valid reference. ExtractorCache is thread-safe.
 *
 * @details
 * @code
 * ExtractorVisitor<std::string> visitor;
 * auto result = boost::apply_visitor(visitor, fieldValue);
 * @endcode
 */
template <typename T>
struct ExtractorVisitor
    : public boost::static_visitor<cpp::result<std::reference_wrapper<const T>, std::optional<std::string>>> {
  cpp::result<std::reference_wrapper<const T>, std::optional<std::string>> operator()(const std::string& value) const {
    using namespace vrml_proc::core::logger;
    using namespace vrml_proc::core::utils;

    LogDebug(FormatString("Visit string. Object address is <", &value, ">."), LOGGING_INFO);

    if constexpr (std::is_same<T, std::string>::value) {
      LogDebug("Extract as string.", LOGGING_INFO);
      return std::cref(value);
    }

    LogDebug("String could not be extracted.", LOGGING_INFO);
    return cpp::fail(std::optional<std::string>(TypeToString<std::string>()));
  }

  cpp::result<std::reference_wrapper<const T>, std::optional<std::string>> operator()(const bool& value) const {
    vrml_proc::core::logger::Log(vrml_proc::core::utils::FormatString("Visit bool. Object address is <", &value, ">."),
        vrml_proc::core::logger::Level::Debug, LOGGING_INFO);

    if constexpr (std::is_same<T, bool>::value) {
      vrml_proc::core::logger::Log("Extract as bool.", vrml_proc::core::logger::Level::Debug, LOGGING_INFO);
      return std::cref(value);
    }

    vrml_proc::core::logger::Log("Bool could not be extracted.", vrml_proc::core::logger::Level::Debug, LOGGING_INFO);
    return cpp::fail(std::optional<std::string>(vrml_proc::core::utils::TypeToString<bool>()));
  }

  cpp::result<std::reference_wrapper<const T>, std::optional<std::string>> operator()(
      const vrml_proc::parser::Vec3fArray& value) const {
    vrml_proc::core::logger::Log(
        vrml_proc::core::utils::FormatString("Visit Vec3fArray. Object address is <", &value, ">."),
        vrml_proc::core::logger::Level::Debug, LOGGING_INFO);

    if (value.vectors.size() == 0) {
      vrml_proc::core::logger::Log("Array is empty.", vrml_proc::core::logger::Level::Debug, LOGGING_INFO);

      if constexpr (std::is_same<T, vrml_proc::parser::Vec3fArray>::value) {
        vrml_proc::core::logger::Log("Extract as Vec3fArray.", vrml_proc::core::logger::Level::Debug, LOGGING_INFO);
        return std::cref(value);
      }

      else if constexpr (std::is_same<T, vrml_proc::parser::Vec2fArray>::value) {
        static const vrml_proc::parser::Vec2fArray emptyVec2fArray{};
        vrml_proc::core::logger::Log(
            vrml_proc::core::utils::FormatString(
                "Extract as Vec2fArray. Using static empty object with address <", &emptyVec2fArray, ">!"),
            vrml_proc::core::logger::Level::Debug, LOGGING_INFO);
        return std::cref(emptyVec2fArray);
      }

      else if constexpr (std::is_same<T, vrml_proc::parser::Int32Array>::value) {
        static const vrml_proc::parser::Int32Array emptyInt32Array{};
        vrml_proc::core::logger::Log(
            vrml_proc::core::utils::FormatString(
                "Extract as Int32Array. Using static empty object with address <", &emptyInt32Array, ">!"),
            vrml_proc::core::logger::Level::Debug, LOGGING_INFO);
        return std::cref(emptyInt32Array);
      }

      else if constexpr (std::is_same<T,
                             std::vector<boost::variant<boost::recursive_wrapper<vrml_proc::parser::VrmlNode>,
                                 boost::recursive_wrapper<vrml_proc::parser::UseNode>>>>::value) {
        static const std::vector<boost::variant<boost::recursive_wrapper<vrml_proc::parser::VrmlNode>,
            boost::recursive_wrapper<vrml_proc::parser::UseNode>>>
            emptyVrmlNodeArray{};
        vrml_proc::core::logger::Log(
            vrml_proc::core::utils::FormatString(
                "Extract as VrmlNodeArray. Using static empty object with address <", &emptyVrmlNodeArray, ">!"),
            vrml_proc::core::logger::Level::Debug, LOGGING_INFO);
        return std::cref(emptyVrmlNodeArray);
      }

      else {
        vrml_proc::core::logger::Log(
            "Empty array type could not be extracted.", vrml_proc::core::logger::Level::Debug, LOGGING_INFO);
        return cpp::fail(std::optional<std::string>{"Requested type does not match any supported empty array type."});
      }
    }

    if constexpr (std::is_same<T, vrml_proc::parser::Vec3fArray>::value) {
      vrml_proc::core::logger::Log("Extract as Vec3fArray.", vrml_proc::core::logger::Level::Debug, LOGGING_INFO);
      return std::cref(value);
    }

    vrml_proc::core::logger::Log(
        "Vec3fArray could not be extracted.", vrml_proc::core::logger::Level::Debug, LOGGING_INFO);
    return cpp::fail(std::optional<std::string>(vrml_proc::core::utils::TypeToString<vrml_proc::parser::Vec3fArray>()));
  }

  cpp::result<std::reference_wrapper<const T>, std::optional<std::string>> operator()(
      const vrml_proc::parser::Vec2fArray& value) const {
    vrml_proc::core::logger::Log(
        vrml_proc::core::utils::FormatString("Visit Vec2fArray. Object address is <", &value, ">."),
        vrml_proc::core::logger::Level::Debug, LOGGING_INFO);

    if constexpr (std::is_same<T, vrml_proc::parser::Vec2fArray>::value) {
      vrml_proc::core::logger::Log("Extract as Vec2fArray.", vrml_proc::core::logger::Level::Debug, LOGGING_INFO);
      return std::cref(value);
    }

    vrml_proc::core::logger::Log(
        "Vec2fArray could not be extracted.", vrml_proc::core::logger::Level::Debug, LOGGING_INFO);
    return cpp::fail(std::optional<std::string>(vrml_proc::core::utils::TypeToString<vrml_proc::parser::Vec2fArray>()));
  }

  cpp::result<std::reference_wrapper<const T>, std::optional<std::string>> operator()(
      const vrml_proc::parser::Int32Array& value) const {
    vrml_proc::core::logger::Log(
        vrml_proc::core::utils::FormatString("Visit Int32Array. Object address is <", &value, ">."),
        vrml_proc::core::logger::Level::Debug, LOGGING_INFO);

    if constexpr (std::is_same<T, vrml_proc::parser::Int32Array>::value) {
      vrml_proc::core::logger::Log("Extract as Int32Array.", vrml_proc::core::logger::Level::Debug, LOGGING_INFO);
      return std::cref(value);
    }

    vrml_proc::core::logger::Log(
        "Int32Array could not be extracted.", vrml_proc::core::logger::Level::Debug, LOGGING_INFO);
    return cpp::fail(std::optional<std::string>(vrml_proc::core::utils::TypeToString<vrml_proc::parser::Int32Array>()));
  }

  cpp::result<std::reference_wrapper<const T>, std::optional<std::string>> operator()(const float& value) const {
    using namespace vrml_proc::core::logger;
    using namespace vrml_proc::core::utils;
    using namespace vrml_proc::parser::model::utils;

    LogDebug(FormatString("Visit float32_t. Object address is <", &value, ">."), LOGGING_INFO);

    if constexpr (std::is_same<T, float>::value) {
      LogDebug("Extract as float32_t.", LOGGING_INFO);
      return std::cref(value);
    }

    if constexpr (std::is_same<T, int32_t>::value) {
      if (value == static_cast<int32_t>(value)) {
        static ExtractorCache cache;
        auto intValue = cache.GetInt(reinterpret_cast<std::uintptr_t>(&value));

        if (intValue.has_value()) {
          LogDebug(FormatString(
                       "Edge case: float32_t is a whole number and thus can be extracted as int32_t with address: <",
                       &(intValue.value()), ">."),
              LOGGING_INFO);
          return intValue.value();
        } else {
          cache.StoreInt(reinterpret_cast<std::uintptr_t>(&value), static_cast<int32_t>(value));
          auto newIntValue = cache.GetInt(reinterpret_cast<std::uintptr_t>(&value)).value();
          LogDebug(FormatString(
                       "Edge case: float32_t is a whole number and thus can be extracted as int32_t with address: <",
                       &(newIntValue), ">."),
              LOGGING_INFO);
          return newIntValue;
        }
      }
    }

    LogDebug("Float32_t could not be extracted.", LOGGING_INFO);
    return cpp::fail(std::optional<std::string>(TypeToString<float>()));
  }

  cpp::result<std::reference_wrapper<const T>, std::optional<std::string>> operator()(const int32_t& value) const {
    using namespace vrml_proc::core::logger;
    using namespace vrml_proc::core::utils;
    using namespace vrml_proc::parser::model::utils;

    LogDebug(FormatString("Visit int32_t. Object address is <", &value, ">."), LOGGING_INFO);

    if constexpr (std::is_same<T, int32_t>::value) {
      LogDebug("Extract as int32_t.", LOGGING_INFO);
      return std::cref(value);
    }

    if constexpr (std::is_same<T, float>::value) {
      if (value == static_cast<float>(value)) {
        static ExtractorCache cache;
        auto floatValue = cache.GetFloat(reinterpret_cast<std::uintptr_t>(&value));

        if (floatValue.has_value()) {
          LogDebug(FormatString("Edge case: int32_t can be expressed and extracted as static float32_t with address: <",
                       &(floatValue.value()), ">."),
              LOGGING_INFO);
          return floatValue.value();
        } else {
          cache.StoreFloat(reinterpret_cast<std::uintptr_t>(&value), static_cast<vrml_proc::parser::float32_t>(value));
          auto newFloatValue = cache.GetFloat(reinterpret_cast<std::uintptr_t>(&value)).value();
          LogDebug(FormatString("Edge case: int32_t can be expressed and extracted as static float32_t with address: <",
                       &(newFloatValue), ">."),
              LOGGING_INFO);
          return newFloatValue;
        }
      }
    }

    LogDebug("Int32_t could not be extracted.", LOGGING_INFO);
    return cpp::fail(std::optional<std::string>(TypeToString<int32_t>()));
  }

  cpp::result<std::reference_wrapper<const T>, std::optional<std::string>> operator()(
      const vrml_proc::parser::Vec2f& value) const {
    vrml_proc::core::logger::Log(vrml_proc::core::utils::FormatString("Visit Vec2f. Object address is <", &value, ">."),
        vrml_proc::core::logger::Level::Debug, LOGGING_INFO);

    if constexpr (std::is_same<T, vrml_proc::parser::Vec2f>::value) {
      vrml_proc::core::logger::Log("Extract as Vec2f.", vrml_proc::core::logger::Level::Debug, LOGGING_INFO);
      return std::cref(value);
    }

    vrml_proc::core::logger::Log("Vec2f could not be extracted.", vrml_proc::core::logger::Level::Debug, LOGGING_INFO);
    return cpp::fail(std::optional<std::string>(vrml_proc::core::utils::TypeToString<vrml_proc::parser::Vec2f>()));
  }

  cpp::result<std::reference_wrapper<const T>, std::optional<std::string>> operator()(
      const vrml_proc::parser::Vec3f& value) const {
    vrml_proc::core::logger::Log(vrml_proc::core::utils::FormatString("Visit Vec3f. Object address is <", &value, ">."),
        vrml_proc::core::logger::Level::Debug, LOGGING_INFO);

    if constexpr (std::is_same<T, vrml_proc::parser::Vec3f>::value) {
      vrml_proc::core::logger::Log("Extract as Vec3f.", vrml_proc::core::logger::Level::Debug, LOGGING_INFO);
      return std::cref(value);
    }

    vrml_proc::core::logger::Log("Vec3f could not be extracted.", vrml_proc::core::logger::Level::Debug, LOGGING_INFO);
    return cpp::fail(std::optional<std::string>(vrml_proc::core::utils::TypeToString<vrml_proc::parser::Vec3f>()));
  }

  cpp::result<std::reference_wrapper<const T>, std::optional<std::string>> operator()(
      const vrml_proc::parser::Vec4f& value) const {
    vrml_proc::core::logger::Log(vrml_proc::core::utils::FormatString("Visit Vec4f. Object address is <", &value, ">."),
        vrml_proc::core::logger::Level::Debug, LOGGING_INFO);

    if constexpr (std::is_same<T, vrml_proc::parser::Vec4f>::value) {
      vrml_proc::core::logger::Log("Extract as Vec4f.", vrml_proc::core::logger::Level::Debug, LOGGING_INFO);
      return std::cref(value);
    }

    vrml_proc::core::logger::Log("Vec4f could not be extracted.", vrml_proc::core::logger::Level::Debug, LOGGING_INFO);
    return cpp::fail(std::optional<std::string>(vrml_proc::core::utils::TypeToString<vrml_proc::parser::Vec4f>()));
  }

  cpp::result<std::reference_wrapper<const T>, std::optional<std::string>> operator()(
      const vrml_proc::parser::UseNode& value) const {
    vrml_proc::core::logger::Log(
        vrml_proc::core::utils::FormatString("Visit UseNode (non-recursive). Object address is <", &value, ">."),
        vrml_proc::core::logger::Level::Debug, LOGGING_INFO);

    if constexpr (std::is_same<T, vrml_proc::parser::UseNode>::value) {
      vrml_proc::core::logger::Log(
          "Extract as UseNode (non-recursive).", vrml_proc::core::logger::Level::Debug, LOGGING_INFO);
      return std::cref(value);
    }

    vrml_proc::core::logger::Log(
        "UseNode (non-recursive) could not be extracted.", vrml_proc::core::logger::Level::Debug, LOGGING_INFO);
    return cpp::fail(std::optional<std::string>(vrml_proc::core::utils::TypeToString<vrml_proc::parser::UseNode>()));
  }

  cpp::result<std::reference_wrapper<const T>, std::optional<std::string>> operator()(
      const boost::recursive_wrapper<vrml_proc::parser::UseNode>& node) const {
    vrml_proc::core::logger::Log(
        vrml_proc::core::utils::FormatString("Visit UseNode (recursive). Object address is <", &node, ">."),
        vrml_proc::core::logger::Level::Debug, LOGGING_INFO);

    if constexpr (std::is_same<T, vrml_proc::parser::UseNode>::value) {
      vrml_proc::core::logger::Log(
          "Extract as UseNode (recursive).", vrml_proc::core::logger::Level::Debug, LOGGING_INFO);
      return std::cref(node);
    }

    vrml_proc::core::logger::Log(
        "UseNode (recursive) could not be extracted.", vrml_proc::core::logger::Level::Debug, LOGGING_INFO);
    return cpp::fail(std::optional<std::string>(
        vrml_proc::core::utils::TypeToString<boost::recursive_wrapper<vrml_proc::parser::UseNode>>()));
  }

  cpp::result<std::reference_wrapper<const T>, std::optional<std::string>> operator()(
      const boost::recursive_wrapper<vrml_proc::parser::VrmlNode>& node) const {
    vrml_proc::core::logger::Log(
        vrml_proc::core::utils::FormatString("Visit VrmlNode (recursive). Object address is <", &node, ">."),
        vrml_proc::core::logger::Level::Debug, LOGGING_INFO);

    if constexpr (std::is_same<T, boost::recursive_wrapper<vrml_proc::parser::VrmlNode>>::value) {
      vrml_proc::core::logger::Log(
          "Extract as VrmlNode (recursive).", vrml_proc::core::logger::Level::Debug, LOGGING_INFO);
      return std::cref(node);
    }

    vrml_proc::core::logger::Log(
        "VrmlNode (recursive) could not be extracted.", vrml_proc::core::logger::Level::Debug, LOGGING_INFO);
    return cpp::fail(std::optional<std::string>(vrml_proc::core::utils::TypeToString<vrml_proc::parser::VrmlNode>()));
  }

  cpp::result<std::reference_wrapper<const T>, std::optional<std::string>> operator()(
      const vrml_proc::parser::VrmlNode& node) const {
    vrml_proc::core::logger::Log(
        vrml_proc::core::utils::FormatString("Visit VrmlNode (non-recursive). Object address is <", &node, ">."),
        vrml_proc::core::logger::Level::Debug, LOGGING_INFO);

    if constexpr (std::is_same<T, vrml_proc::parser::VrmlNode>::value) {
      vrml_proc::core::logger::Log(
          "Extract as VrmlNode (non-recursive).", vrml_proc::core::logger::Level::Debug, LOGGING_INFO);
      return std::cref(node);
    }

    /*if constexpr (std::is_same<T, boost::recursive_wrapper<vrml_proc::parser::VrmlNode>>::value) {
        return std::cref(boost::recursive_wrapper<vrml_proc::parser::VrmlNode>(node));
    }*/

    vrml_proc::core::logger::Log(
        "VrmlNode (non-recursive) could not be extracted.", vrml_proc::core::logger::Level::Debug, LOGGING_INFO);
    return cpp::fail(std::optional<std::string>(vrml_proc::core::utils::TypeToString<vrml_proc::parser::VrmlNode>()));
  }

  cpp::result<std::reference_wrapper<const T>, std::optional<std::string>> operator()(
      const std::vector<boost::variant<boost::recursive_wrapper<vrml_proc::parser::VrmlNode>,
          boost::recursive_wrapper<vrml_proc::parser::UseNode>>>& value) const {
    vrml_proc::core::logger::Log(
        vrml_proc::core::utils::FormatString("Visit VrmlNodeArray. Object address is <", &value, ">."),
        vrml_proc::core::logger::Level::Debug, LOGGING_INFO);

    if constexpr (std::is_same<T, std::vector<boost::variant<boost::recursive_wrapper<vrml_proc::parser::VrmlNode>,
                                      boost::recursive_wrapper<vrml_proc::parser::UseNode>>>>::value) {
      vrml_proc::core::logger::Log("Extract as VrmlNodeArray.", vrml_proc::core::logger::Level::Debug, LOGGING_INFO);
      return std::cref(value);
    }

    vrml_proc::core::logger::Log(
        "VrmlNodeArray could not be extracted.", vrml_proc::core::logger::Level::Debug, LOGGING_INFO);
    return cpp::fail(std::optional<std::string>(vrml_proc::core::utils::TypeToString<
        std::vector<boost::variant<boost::recursive_wrapper<vrml_proc::parser::VrmlNode>,
            boost::recursive_wrapper<vrml_proc::parser::UseNode>>>>()));
  }

  template <typename U>
  cpp::result<std::reference_wrapper<const T>, std::optional<std::string>> operator()(const U&) const {
    vrml_proc::core::logger::Log(vrml_proc::core::utils::FormatString("Type mismatch! Expected <", typeid(T).name(),
                                     ">,  but received <", typeid(U).name(), ">."),
        vrml_proc::core::logger::Level::Debug, LOGGING_INFO);
    return cpp::fail(std::optional<std::string>(vrml_proc::core::utils::TypeToString<U>()));
  }
};

template <typename T>
struct VariantVisitor
    : public boost::static_visitor<cpp::result<std::reference_wrapper<const T>, std::optional<std::string>>> {
  cpp::result<std::reference_wrapper<const T>, std::optional<std::string>> operator()(
      const vrml_proc::parser::UseNode& node) const {
    vrml_proc::core::logger::Log(
        vrml_proc::core::utils::FormatString("Visit UseNode (non-recursive). Object address is <", &node, ">."),
        vrml_proc::core::logger::Level::Debug, LOGGING_INFO);

    if constexpr (std::is_same<T, vrml_proc::parser::UseNode>::value) {
      vrml_proc::core::logger::Log(
          "Extract as UseNode (non-recursive).", vrml_proc::core::logger::Level::Debug, LOGGING_INFO);
      return std::cref(node);
    }

    /*if constexpr (std::is_same<T, boost::recursive_wrapper<vrml_proc::parser::UseNode>>::value) {
        return std::cref(boost::recursive_wrapper<vrml_proc::parser::UseNode>(node));
    }*/

    vrml_proc::core::logger::Log(
        "UseNode (non-recursive) could not be extracted.", vrml_proc::core::logger::Level::Debug, LOGGING_INFO);
    return cpp::fail(std::optional<std::string>(vrml_proc::core::utils::TypeToString<vrml_proc::parser::UseNode>()));
  }

  cpp::result<std::reference_wrapper<const T>, std::optional<std::string>> operator()(
      const boost::recursive_wrapper<vrml_proc::parser::UseNode>& node) const {
    vrml_proc::core::logger::Log(
        vrml_proc::core::utils::FormatString("Visit UseNode (recursive). Object address is <", &node, ">."),
        vrml_proc::core::logger::Level::Debug, LOGGING_INFO);

    if constexpr (std::is_same<T, boost::recursive_wrapper<vrml_proc::parser::UseNode>>::value) {
      vrml_proc::core::logger::Log(
          "Extract as UseNode (recursive).", vrml_proc::core::logger::Level::Debug, LOGGING_INFO);
      return std::cref(node.get());
    }

    vrml_proc::core::logger::Log(
        "UseNode (recursive) could not be extracted.", vrml_proc::core::logger::Level::Debug, LOGGING_INFO);
    return cpp::fail(std::optional<std::string>(
        vrml_proc::core::utils::TypeToString<boost::recursive_wrapper<vrml_proc::parser::UseNode>>()));
  }

  cpp::result<std::reference_wrapper<const T>, std::optional<std::string>> operator()(
      const boost::recursive_wrapper<vrml_proc::parser::VrmlNode>& node) const {
    vrml_proc::core::logger::Log(
        vrml_proc::core::utils::FormatString("Visit VrmlNode (recursive). Object address is <", &node, ">."),
        vrml_proc::core::logger::Level::Debug, LOGGING_INFO);

    if constexpr (std::is_same<T, boost::recursive_wrapper<vrml_proc::parser::VrmlNode>>::value) {
      vrml_proc::core::logger::Log(
          "Extract as VrmlNode (recursive).", vrml_proc::core::logger::Level::Debug, LOGGING_INFO);
      return std::cref(node.get());
    }

    vrml_proc::core::logger::Log(
        "VrmlNode (recursive) could not be extracted.", vrml_proc::core::logger::Level::Debug, LOGGING_INFO);
    return cpp::fail(std::optional<std::string>(
        vrml_proc::core::utils::TypeToString<boost::recursive_wrapper<vrml_proc::parser::VrmlNode>>()));
  }

  cpp::result<std::reference_wrapper<const T>, std::optional<std::string>> operator()(
      const vrml_proc::parser::VrmlNode& node) const {
    vrml_proc::core::logger::Log(
        vrml_proc::core::utils::FormatString("Visit VrmlNode (non-recursive). Object address is <", &node, ">."),
        vrml_proc::core::logger::Level::Debug, LOGGING_INFO);

    if constexpr (std::is_same<T, vrml_proc::parser::VrmlNode>::value) {
      vrml_proc::core::logger::Log(
          "Extract as VrmlNode (non-recursive).", vrml_proc::core::logger::Level::Debug, LOGGING_INFO);
      return std::cref(node);
    }

    /*if constexpr (std::is_same<T, boost::recursive_wrapper<vrml_proc::parser::VrmlNode>>::value) {
        return std::cref(boost::recursive_wrapper<vrml_proc::parser::VrmlNode>(node));
    }*/

    vrml_proc::core::logger::Log(
        "VrmlNode (non-recursive) could not be extracted.", vrml_proc::core::logger::Level::Debug, LOGGING_INFO);
    return cpp::fail(std::optional<std::string>(vrml_proc::core::utils::TypeToString<vrml_proc::parser::VrmlNode>()));
  }

  template <typename U>
  cpp::result<std::reference_wrapper<const T>, std::optional<std::string>> operator()(const U&) const {
    vrml_proc::core::logger::Log(vrml_proc::core::utils::FormatString("Type mismatch! Expected <", typeid(T).name(),
                                     ">,  but received <", typeid(U).name(), ">."),
        vrml_proc::core::logger::Level::Debug, LOGGING_INFO);
    return cpp::fail(std::optional<std::string>(vrml_proc::core::utils::TypeToString<U>()));
  }
};
