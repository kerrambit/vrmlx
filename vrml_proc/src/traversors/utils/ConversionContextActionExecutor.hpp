#pragma once

#include <memory>
#include <string>
#include <vector>

#include "ConversionContextable.hpp"
#include "ConversionContextActionMap.hpp"
#include "HandlerToActionBundle.hpp"

namespace vrml_proc::traversor::utils::ConversionContextActionExecutor {

  /**
   * @brief Tries to execute an action. Action to execute is chosen by `key` from `actionMap`.
   * If no action is found, default ConversionContext is returned.
   *
   * @param actionMap map storing all actions
   * @param key string which chooses action from the map
   * @param data object which will be passed into action constructor when creating action instance from the map
   */
  template <vrml_proc::core::contract::ConversionContextable ConversionContext>
  static std::shared_ptr<ConversionContext> TryToExecute(
      const vrml_proc::action::ConversionContextActionMap<ConversionContext>& actionMap,
      const std::string& key,
      vrml_proc::traversor::handler::HandlerToActionBundle<ConversionContext> data) {  //

    if (actionMap.VerifyKey(key)) {
      std::shared_ptr<ConversionContext> result = actionMap.GetAction(key, data)->Execute();
      auto derivedContext = std::dynamic_pointer_cast<ConversionContext>(result);
      if (derivedContext != nullptr) {
        return derivedContext;
      }
    }
    return std::make_shared<ConversionContext>();
  }
}  // namespace vrml_proc::traversor::utils::ConversionContextActionExecutor
