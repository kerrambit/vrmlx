#pragma once

#include <any>
#include <functional>
#include <memory>
#include <string>

#include "ConversionContextAction.hpp"
#include "FormatString.hpp"
#include "HandlerToActionBundle.hpp"
#include "Logger.hpp"
#include "ConversionContextable.hpp"

namespace vrml_proc::action {
  /**
   * @brief Represents class which is responsible for storing and mapping `Action` functor value to a string key.
   */
  template <ConversionContextable ConversionContext>
  class ConversionContextActionMap {
   public:
    /**
     * @brief Argument represented by a HandlerToActionBundle object.
     */
    using Argument = vrml_proc::traversor::handler::HandlerToActionBundle<ConversionContext>;

    /**
     * @brief The basic data type stored in the ActionMap. It is a function pointer which takes a arguments
     * (represented by VRML node data) and returns a shared pointer of ConversionContextAction.
     */
    using ActionFunctor = std::function<std::shared_ptr<ConversionContextAction<ConversionContext>>(Argument)>;

    /**
     * @brief Adds new action to the ActionMap mapped to a string key.
     * Duplicated key will be rewritten.
     *
     * @param key key which will be mapped to the Action
     * @param action ActionFunctor representing the given Action
     */
    void AddAction(const std::string& key, ActionFunctor action) {  //

      vrml_proc::core::logger::LogDebug(
          vrml_proc::core::utils::FormatString("Add new action with key <", key, ">."), LOGGING_INFO);
      m_actions[key] = std::move(action);
    }

    /**
     * @brief Verifies if the given key exists in the ActionMap.
     *
     * @param key key identifying the given Action
     * @return true if the key is present, false if the key is not present.
     */
    bool VerifyKey(const std::string& key) const {  //

      bool result = (m_actions.find(key) != m_actions.end());
      vrml_proc::core::logger::LogTrace(vrml_proc::core::utils::FormatString("Verify key <", key, ">."), LOGGING_INFO);
      if (result) {
        vrml_proc::core::logger::LogTrace(
            vrml_proc::core::utils::FormatString("Key <", key, "> was not found!"), LOGGING_INFO);
      }
      return result;
    }

    /**
     * @brief Gets a shared pointer owning a ConversionContextAction object. The function creates new object using
     * functor stored in the ActionMap and passed `arg` which is passed in to the Action constructor.
     *
     * @param key key identifying the given Action
     * @param arg argument passed into the function responsible for creating the given Action
     * @returns nullptr if the key does not exists, otherwise shared pointer owning the given ConversionContextAction
     * object
     */
    std::shared_ptr<ConversionContextAction<ConversionContext>> GetAction(
        const std::string& key, Argument arg) const {  //

      vrml_proc::core::logger::LogDebug(
          vrml_proc::core::utils::FormatString("Retrieve action by key <", key, ">."), LOGGING_INFO);

      auto iterator = m_actions.find(key);
      if (iterator != m_actions.end()) {
        return iterator->second(arg);
      }

      vrml_proc::core::logger::LogWarning(
          vrml_proc::core::utils::FormatString("Action with key <", key, "> was not found!"), LOGGING_INFO);

      return nullptr;
    }

   private:
    std::unordered_map<std::string, ActionFunctor> m_actions;
  };
}  // namespace vrml_proc::action
