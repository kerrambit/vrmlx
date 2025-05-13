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

// Forward declaration.
template <typename T>
struct ExtractorVisitor;

// Forward declaration.
template <typename T>
struct VariantVisitor;

/**
 * @brief Represents low-level functions used for extracting and manipulating with VrmlField and VrmlFieldValue types.
 *
 * These types are defined simply as variants of many different possible values mirroring the VRML 2.0 standart. These
 * structs are left barebone, and thus addtional utils functions are needed to manipulation with them.
 */
namespace vrml_proc::parser::model::utils::VrmlFieldExtractor {

  /**
   * @brief Checks if the field name is present in list of fields.
   *
   * @param name name to find
   * @param fields list of fields
   * @returns true if the name was found, otherwise false
   */
  inline bool IsNamePresent(const std::string& name, const std::vector<vrml_proc::parser::VrmlField>& fields) {
    for (const auto& field : fields) {
      if (field.name == name) {
        return true;
      }
    }
    return false;
  }

  // --------------------------------------------------------

  /**
   * @brief Extracts T value from the variant.
   *
   * @tparam T type to extract
   * @param fieldValue field value (variant)
   * @param invalidType out parameter, which is filled with invalid type name, if T cannot be extracted from
   * `fieldValue`
   * @returns optional storing the reference to the const T value from variant if T can be extracted, otherwise nullopt
   */
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

  /**
   * @brief Extracts T value from the variant.
   *
   * @tparam T type to extract
   * @param fieldValue field value (variant)
   * @returns optional storing the reference to the const T value from variant if T can be extracted, otherwise nullopt
   */
  template <typename T>
  inline std::optional<std::reference_wrapper<const T>> Extract(const vrml_proc::parser::VrmlFieldValue& fieldValue) {
    std::string invalidType;
    return vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractExtended<T>(fieldValue, invalidType);
  }

  // --------------------------------------------------------

  /**
   * @brief Represents possible result of ExtractByName() function.
   */
  enum class ExtractByNameError { FieldNotFound, ValidationError };

  /**
   * @brief Attempts to extract a field of a specific type T by name.
   *
   * This function searches for a field with a given name in the provided list of VRML fields.
   * If the field is found and its type matches the expected type `T`, it returns a reference
   * to the value. Otherwise, it returns an appropriate `ExtractByNameError`.
   *
   * If a validation error occurs (i.e., the field is found but its type is incorrect),
   * the `invalidType` string will be updated with a invalid type of the mismatch.
   *
   * @tparam T expected type of the field value to extract
   * @param name name of the field to look for
   * @param fields list of available VRML fields
   * @param invalidType string to receive the name of the mismatched type if validation fails
   * @return a `cpp::result` containing a reference to the extracted value if successful,
   *         or an `ExtractByNameError` on failure
   */
  template <typename T>
  inline cpp::result<std::reference_wrapper<const T>, ExtractByNameError> ExtractByNameExtended(
      const std::string& name, const std::vector<vrml_proc::parser::VrmlField>& fields, std::string& invalidType) {  //

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

  /**
   * @brief Attempts to extract a field of a specific type T by name.
   *
   * This function searches for a field with a given name in the provided list of VRML fields.
   * If the field is found and its type matches the expected type `T`, it returns a reference
   * to the value. Otherwise, it returns an appropriate `ExtractByNameError`.
   *
   * @tparam T expected type of the field value to extract
   * @param name name of the field to look for
   * @param fields list of available VRML fields
   * @return a `cpp::result` containing a reference to the extracted value if successful,
   *         or an `ExtractByNameError` on failure
   */
  template <typename T>
  inline cpp::result<std::reference_wrapper<const T>, ExtractByNameError> ExtractByName(
      const std::string& name, const std::vector<vrml_proc::parser::VrmlField>& fields) {
    std::string invalidType;
    return ExtractByNameExtended<T>(name, fields, invalidType);
  }

  // --------------------------------------------------------

  /**
   * @brief Represents possible results of the ExtractVrmlNode() function.
   */
  enum class ExtractVrmlNodeError { FieldNotFound, ValidationError, UnknownUseNode };

  /**
   * @brief Extracts a VRML node or resolves a `USE` reference from a field list.
   *
   * This function attempts to extract a `VrmlNode` with the given name from the provided list of fields.
   * If a matching node of the correct type is not found, it checks whether the name refers to a `UseNode`
   * and attempts to resolve the reference via the `VrmlNodeManager`.
   *
   * On failure due to a type mismatch, the `invalidType` string is populated. If the node is a `USE`
   * reference that could not be resolved, the `useId` string is populated with the unresolved identifier.
   *
   * @param name name of the node field to search for
   * @param fields list of available VRML fields
   * @param manager reference to the `VrmlNodeManager` used to resolve `USE` references
   * @param invalidType will be populated with type error information if type validation fail
   * @param useId will be populated with the unresolved identifier if a `USE` reference fails to resolve.
   * @return a `cpp::result` containing a reference to the resolved `VrmlNode`, or an `ExtractVrmlNodeError` on failure
   */
  inline cpp::result<std::reference_wrapper<const vrml_proc::parser::VrmlNode>, ExtractVrmlNodeError>
  ExtractVrmlNodeExtended(const std::string& name,
      const std::vector<vrml_proc::parser::VrmlField>& fields,
      const vrml_proc::parser::service::VrmlNodeManager& manager,
      std::string& invalidType,
      std::string& useId) {  //

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
    }

    return cpp::fail(ExtractVrmlNodeError::FieldNotFound);
  }

  /**
   * @brief Extracts a VRML node or resolves a `USE` reference from a field list.
   *
   * This function attempts to extract a `VrmlNode` with the given name from the provided list of fields.
   * If a matching node of the correct type is not found, it checks whether the name refers to a `UseNode`
   * and attempts to resolve the reference via the `VrmlNodeManager`.
   *
   * @param name name of the node field to search for
   * @param fields list of available VRML fields
   * @param manager reference to the `VrmlNodeManager` used to resolve `USE` references
   * @return a `cpp::result` containing a reference to the resolved `VrmlNode`, or an `ExtractVrmlNodeError` on failure
   */
  inline cpp::result<std::reference_wrapper<const vrml_proc::parser::VrmlNode>, ExtractVrmlNodeError> ExtractVrmlNode(
      const std::string& name,
      const std::vector<vrml_proc::parser::VrmlField>& fields,
      const vrml_proc::parser::service::VrmlNodeManager& manager) {
    std::string invalidType;
    std::string useId;
    return ExtractVrmlNodeExtended(name, fields, manager, invalidType, useId);
  }

  // --------------------------------------------------------

  /**
   * @brief Attempts to extract a `VrmlNode` or `UseNode` from a variant without resolving `USE` references.
   *
   * This function extracts  a variant that may contain either a `VrmlNode` or
   * `UseNode`. This function does not attempt to resolve `USE` references via a node manager.
   *
   * If the extraction fails due to a type mismatch, `invalidType` is populated with the error description.
   *
   * @tparam T expected type of VRML node to extract (`VrmlNode` or `UseNode`)
   * @param variant variant potentially containing a `VrmlNode` or `UseNode`
   * @param invalidType qill be populated with error details if extraction fails
   * @return an `std::optional` containing a reference to the extracted object if successful; otherwise, nullopt
   */
  template <typename T>
  inline std::optional<std::reference_wrapper<const T>> ExtractVrmlNodeFromVariantWithoutResolvingExtended(
      const boost::variant<boost::recursive_wrapper<vrml_proc::parser::VrmlNode>,
          boost::recursive_wrapper<vrml_proc::parser::UseNode>>& variant,
      std::string& invalidType) {  //

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

  /**
   * @brief Attempts to extract a `VrmlNode` or `UseNode` from a variant without resolving `USE` references.
   *
   * This function extracts  a variant that may contain either a `VrmlNode` or
   * `UseNode`. This function does not attempt to resolve `USE` references via a node manager.
   *
   * @tparam T expected type of VRML node to extract (`VrmlNode` or `UseNode`)
   * @param variant variant potentially containing a `VrmlNode` or `UseNode`
   * @return an `std::optional` containing a reference to the extracted object if successful; otherwise, nullopt
   */
  template <typename T>
  inline std::optional<std::reference_wrapper<const T>> ExtractVrmlNodeFromVariantWithoutResolving(
      const boost::variant<boost::recursive_wrapper<vrml_proc::parser::VrmlNode>,
          boost::recursive_wrapper<vrml_proc::parser::UseNode>>& variant) {
    std::string invalidType;
    return ExtractVrmlNodeFromVariantWithoutResolvingExtended<T>(variant, invalidType);
  }

  // --------------------------------------------------------

  /**
   * @brief Represents possible results of the ExtractVrmlNodeFromVariant() function.
   */
  enum class ExtractVrmlNodeFromVariantError { ValidationError, UnknownUseNode };

  /**
   * @brief Attempts to extract a `VrmlNode` from a variant, resolving `USE` references.
   *
   * This function handles a variant containing either a `VrmlNode` or a `UseNode`. It first tries to extract a
   * `VrmlNode` directly. If the contained type is a `UseNode`, the function attempts to resolve it using the provided
   * `VrmlNodeManager`. If the referenced node cannot be resolved, the function returns an error.
   *
   * @param variant variant that may contain either a `VrmlNode` or a `UseNode`.
   * @param manager manager used to resolve nodes referenced by `USE` statements.
   * @param invalidType will be populated with a string describing the type mismatch or validation issue
   * @param useId will be populated with the identifier from the `UseNode` if the resolution fails
   * @return a `cpp::result` holding either a reference to the resolved `VrmlNode` or an error code indicating failure
   */
  inline cpp::result<std::reference_wrapper<const vrml_proc::parser::VrmlNode>, ExtractVrmlNodeFromVariantError>
  ExtractVrmlNodeFromVariantExtended(const boost::variant<boost::recursive_wrapper<vrml_proc::parser::VrmlNode>,
                                         boost::recursive_wrapper<vrml_proc::parser::UseNode>>& variant,
      const vrml_proc::parser::service::VrmlNodeManager& manager,
      std::string& invalidType,
      std::string& useId) {  //

    std::string out;
    auto result = ExtractVrmlNodeFromVariantWithoutResolvingExtended<vrml_proc::parser::VrmlNode>(variant, out);
    if (result.has_value()) {
      return result.value();
    }

    auto useNode = ExtractVrmlNodeFromVariantWithoutResolvingExtended<vrml_proc::parser::UseNode>(variant, out);
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

  /**
   * @brief Attempts to extract a `VrmlNode` from a variant, resolving `USE` references.
   *
   * This function handles a variant containing either a `VrmlNode` or a `UseNode`. It first tries to extract a
   * `VrmlNode` directly. If the contained type is a `UseNode`, the function attempts to resolve it using the provided
   * `VrmlNodeManager`. If the referenced node cannot be resolved, the function returns an error.
   *
   * @param variant variant that may contain either a `VrmlNode` or a `UseNode`.
   * @param manager manager used to resolve nodes referenced by `USE` statements.
   * @return a `cpp::result` holding either a reference to the resolved `VrmlNode` or an error code indicating failure
   */
  inline cpp::result<std::reference_wrapper<const vrml_proc::parser::VrmlNode>, ExtractVrmlNodeFromVariantError>
  ExtractVrmlNodeFromVariant(const boost::variant<boost::recursive_wrapper<vrml_proc::parser::VrmlNode>,
                                 boost::recursive_wrapper<vrml_proc::parser::UseNode>>& variant,
      const vrml_proc::parser::service::VrmlNodeManager& manager) {
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
 * @note The visitor is thread-safe. It uses only read-only data. There are two exceptions to this. 1. case is
 * for Vec3fArray, where it is needed to examine, if the value is not empty array. If it is an empty array, we check
 * if current requestd type T cannot be reinterpreted as empty array. Possibly we create empty static array in such
 * cases only initialized once (guaranteed by C++11+ thread-safe static initialization). The reason for this is that
 * parser, parsing an emprt array, will always resovle it as Vec3fArray simply because it is the first rule.
 *
 * 2. case is about float/int situation. The grammar handles all (singular!) number without decimal points as ints. The
 * following table shows the problematic behavirour ('X' signs where the special treatment has to be made to be able to
 * e.g. take value '0' as both int and float):
 *
 * number 0 (as int)
 * number 0 (as float) X
 * number 5.0 (as int) X
 * number 5.0 (as float)
 * number 5.5 (as float)
 *
 * The logic is that we convert int to float and then store this value inside cache (see `ExtractorCache` class) or we
 * try to convert float to int (must be in format xyz.0) and also store it. If we encounter the query for the
 * problematic value, we look it up inside the map to retrieve the same valid reference. ExtractorCache is thread-safe.
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

/**
 * @brief Visitor for VRMLNode extraction.
 *
 * @returns result type where value is const reference to queried data entry; if there is error, string describing
 * unexpected error type is returned (or possibly other error message)
 */
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
