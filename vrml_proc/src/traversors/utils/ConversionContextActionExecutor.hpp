#pragma once

#include <any>
#include <memory>
#include <string>
#include <vector>

#include "NodeView.hpp"
#include "HandlerToActionBundle.hpp"

#include "ConversionContextActionMap.hpp"

namespace vrml_proc {
  namespace traversor {
    namespace utils {

      namespace ConversionContextActionExecutor {

        template <typename ConversionContextType>
        static std::shared_ptr<ConversionContextType> TryToExecute(
            const vrml_proc::action::ConversionContextActionMap<ConversionContextType>& actionMap,
            const std::string& key,
            const std::vector<std::reference_wrapper<const std::any>>& refArgs,
            const std::vector<std::any>& copyArgs) {
          if (actionMap.VerifyKey(key)) {
            std::shared_ptr<ConversionContextType> result = actionMap.GetAction(key, refArgs, copyArgs)->Execute();
            auto derivedContext = std::dynamic_pointer_cast<ConversionContextType>(result);
            if (derivedContext != nullptr) {
              return derivedContext;
            }
          }
          return std::make_shared<ConversionContextType>();
        }

        /*template <typename ConversionContextType>
        static std::shared_ptr<ConversionContextType> TryToExecute(
            const vrml_proc::action::ConversionContextActionMap<ConversionContextType>& actionMap,
            const std::string& key,
            std::shared_ptr<vrml_proc::traversor::node_descriptor::NodeView> nodeView) {
          if (actionMap.VerifyKeyForNodeView(key)) {
            std::shared_ptr<ConversionContextType> result = actionMap.GetAction(key, nodeView)->Execute();
            auto derivedContext = std::dynamic_pointer_cast<ConversionContextType>(result);
            if (derivedContext != nullptr) {
              return derivedContext;
            }
          }
          return std::make_shared<ConversionContextType>();
        }*/

        template <typename ConversionContextType>
        static std::shared_ptr<ConversionContextType> TryToExecute(
            const vrml_proc::action::ConversionContextActionMap<ConversionContextType>& actionMap,
            const std::string& key,
            vrml_proc::traversor::handler::HandlerToActionBundle<ConversionContextType> data) {
          if (actionMap.VerifyKeyForHandlerToActionBundle(key)) {
            std::shared_ptr<ConversionContextType> result = actionMap.GetAction(key, data)->Execute();
            auto derivedContext = std::dynamic_pointer_cast<ConversionContextType>(result);
            if (derivedContext != nullptr) {
              return derivedContext;
            }
          }
          return std::make_shared<ConversionContextType>();
        }
      }  // namespace ConversionContextActionExecutor
    }    // namespace utils
  }      // namespace traversor
}  // namespace vrml_proc
