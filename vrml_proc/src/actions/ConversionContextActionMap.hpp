#pragma once

#include <any>
#include <functional>
#include <memory>
#include <string>

#include "ConversionContextAction.hpp"
#include "Logger.hpp"
#include "FormatString.hpp"
#include "NodeView.hpp"
#include "HandlerToActionBundle.hpp"

#include "VrmlProcessingExport.hpp"

namespace vrml_proc {
  namespace action {
    /**
     * @brief Represents class which is responsible for storing and mapping `Action` functor value to a string key.
     */
    template <typename ConversionContext>
    class VRMLPROCESSING_API ConversionContextActionMap {
     public:
      /**
       * @brief Arguments passed into the function responsible for creating the given Action.
       * These argument will be taken as a reference.
       */
      using ReferencedArguments = std::vector<std::reference_wrapper<const std::any>>;
      /**
       * @brief Arguments passed into the function responsible for creating the given Action.
       * These argument will be copied.
       */
      using CopiedArguments = std::vector<std::any>;

      /**
       * @brief Argument represented by a NodeView shared pointer.
       */
      using NodeViewArgument = std::shared_ptr<vrml_proc::traversor::node_descriptor::NodeView>;

      using HandlerToActionBundleArgument = vrml_proc::traversor::handler::HandlerToActionBundle<ConversionContext>;
      /**
       * @brief The basic data type stored in the ActionMap. It is a function pointer to a function, which
       * takes some, for now unknown, arguments and returns a shared pointer owning ConversionContextAction.
       */
      using ActionFunctor = std::function<std::shared_ptr<ConversionContextAction<ConversionContext>>(
          ReferencedArguments, CopiedArguments)>;

      using NodeViewActionFunctor =
          std::function<std::shared_ptr<ConversionContextAction<ConversionContext>>(NodeViewArgument)>;

      using HandlerToActionBundleActionFunctor =
          std::function<std::shared_ptr<ConversionContextAction<ConversionContext>>(HandlerToActionBundleArgument)>;
      /**
       * @brief Adds new action to the ActionMap mapped to a string key.
       * Duplicated key will be rewritten.
       *
       * @param key key which will be mapped to the Action
       * @param action ActionFunctor representing the given Action
       */
      void AddAction(const std::string& key, ActionFunctor action) {
        vrml_proc::core::logger::LogDebug(
            vrml_proc::core::utils::FormatString("Add new action with key <", key, ">."), LOGGING_INFO);
        m_actions[key] = std::move(action);
      }

      void AddAction(const std::string& key, NodeViewActionFunctor action) {
        vrml_proc::core::logger::LogDebug(
            vrml_proc::core::utils::FormatString("Add new action with key <", key, ">."), LOGGING_INFO);
        m_nodeViewActions[key] = std::move(action);
      }

      void AddActionForHandlerToActionBundle(const std::string& key, HandlerToActionBundleActionFunctor action) {
        vrml_proc::core::logger::LogDebug(
            vrml_proc::core::utils::FormatString("Add new action with key <", key, ">."), LOGGING_INFO);
        m_handlerToActionBundleActions[key] = std::move(action);
      }

      /**
       * @brief Verifies if the given key exists in the ActionMap.
       *
       * @param key key identifying the given Action
       * @return true if the key is present, false if the key is not present.
       */
      bool VerifyKey(const std::string& key) const {
        bool result = (m_actions.find(key) != m_actions.end());
        vrml_proc::core::logger::LogTrace(
            vrml_proc::core::utils::FormatString("Verify key <", key, ">."), LOGGING_INFO);
        if (result) {
          vrml_proc::core::logger::LogTrace(
              vrml_proc::core::utils::FormatString("Key <", key, "> was not found!"), LOGGING_INFO);
        }
        return result;
      }

      bool VerifyKeyForNodeView(const std::string& key) const {
        bool result = (m_nodeViewActions.find(key) != m_nodeViewActions.end());
        vrml_proc::core::logger::LogTrace(
            vrml_proc::core::utils::FormatString("Verify key <", key, ">."), LOGGING_INFO);
        if (result) {
          vrml_proc::core::logger::LogTrace(
              vrml_proc::core::utils::FormatString("Key <", key, "> was not found!"), LOGGING_INFO);
        }
        return result;
      }

      bool VerifyKeyForHandlerToActionBundle(const std::string& key) const {
        bool result = (m_handlerToActionBundleActions.find(key) != m_handlerToActionBundleActions.end());
        vrml_proc::core::logger::LogTrace(
            vrml_proc::core::utils::FormatString("Verify key <", key, ">."), LOGGING_INFO);
        if (result) {
          vrml_proc::core::logger::LogTrace(
              vrml_proc::core::utils::FormatString("Key <", key, "> was not found!"), LOGGING_INFO);
        }
        return result;
      }

      /**
       * @brief Gets a shared pointer owning a ConversionContextAction object. The function creates new object using
       * functor stored in the ActionMap and passed `args` which are passed in to the Action constructor.
       *
       * @param key key identifying the given Action
       * @param args arguments passed into the function responsible for creating the given Action
       * @returns nullptr if the key does not exists, otherwise shared pointer owning the given ConversionContextAction
       * object
       */
      std::shared_ptr<ConversionContextAction<ConversionContext>> GetAction(
          const std::string& key, const ReferencedArguments& refArgs, const CopiedArguments& copyArgs) const {
        vrml_proc::core::logger::LogDebug(
            vrml_proc::core::utils::FormatString("Retrieve action by key <", key, ">."), LOGGING_INFO);
        auto iterator = m_actions.find(key);
        if (iterator != m_actions.end()) {
          return iterator->second(refArgs, copyArgs);
        }
        vrml_proc::core::logger::LogWarning(
            vrml_proc::core::utils::FormatString("Action with key <", key, "> was not found!"), LOGGING_INFO);
        return nullptr;
      }

      std::shared_ptr<ConversionContextAction<ConversionContext>> GetAction(
          const std::string& key, NodeViewArgument arg) const {
        vrml_proc::core::logger::LogDebug(
            vrml_proc::core::utils::FormatString("Retrieve action by key <", key, ">."), LOGGING_INFO);
        auto iterator = m_nodeViewActions.find(key);
        if (iterator != m_nodeViewActions.end()) {
          return iterator->second(arg);
        }
        vrml_proc::core::logger::LogWarning(
            vrml_proc::core::utils::FormatString("Action with key <", key, "> was not found!"), LOGGING_INFO);
        return nullptr;
      }

      std::shared_ptr<ConversionContextAction<ConversionContext>> GetAction(
          const std::string& key, HandlerToActionBundleArgument arg) const {
        vrml_proc::core::logger::LogDebug(
            vrml_proc::core::utils::FormatString("Retrieve action by key <", key, ">."), LOGGING_INFO);
        auto iterator = m_handlerToActionBundleActions.find(key);
        if (iterator != m_handlerToActionBundleActions.end()) {
          return iterator->second(arg);
        }
        vrml_proc::core::logger::LogWarning(
            vrml_proc::core::utils::FormatString("Action with key <", key, "> was not found!"), LOGGING_INFO);
        return nullptr;
      }

     private:
      std::unordered_map<std::string, ActionFunctor> m_actions;
      std::unordered_map<std::string, NodeViewActionFunctor> m_nodeViewActions;
      std::unordered_map<std::string, HandlerToActionBundleActionFunctor> m_handlerToActionBundleActions;
    };
  }  // namespace action
}  // namespace vrml_proc
