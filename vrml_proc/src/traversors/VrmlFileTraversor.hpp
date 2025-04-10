#pragma once

#include <memory>

#include "ConversionContextActionMap.hpp"
#include "Error.hpp"
#include "FileTraversorError.hpp"
#include "FormatString.hpp"
#include "Logger.hpp"
#include "ManualTimer.hpp"
#include "NullPointerError.hpp"
#include "VrmlFile.hpp"
#include "VrmlFileTraversorParameters.hpp"
#include "VrmlNodeManager.hpp"
#include "VrmlNodeTraversor.hpp"
#include "VrmlNodeTraversorParameters.hpp"

namespace vrml_proc::traversor::VrmlFileTraversor {

  template <typename ConversionContext>
  inline cpp::result<std::shared_ptr<ConversionContext>, std::shared_ptr<vrml_proc::core::error::Error>> Traverse(
      const VrmlFileTraversorParameters& context,
      const vrml_proc::action::ConversionContextActionMap<ConversionContext>& actionMap) {  //

    using namespace vrml_proc::core::logger;
    using namespace vrml_proc::core::utils;
    using vrml_proc::core::error::NullPointerError;
    using vrml_proc::math::TransformationMatrix;
    using vrml_proc::traversor::error::FileTraversorError;

    LogInfo("Traverse VRML file.", LOGGING_INFO);

    ManualTimer timer;
    timer.Start();

    std::shared_ptr<ConversionContext> traversedFile = std::make_shared<ConversionContext>();

    size_t index = 1;
    for (const auto& root : context.file) {
      LogInfo(FormatString("Found ", index, ". root node. It is type <", root.header, ">."), LOGGING_INFO);

      auto result = vrml_proc::traversor::VrmlNodeTraversor::Traverse<ConversionContext>(
          VrmlNodeTraversorParameters(root, context.manager, false, TransformationMatrix(), context.config), actionMap);

      if (result.has_error()) {
        auto time = timer.End();
        LogError(FormatString("While traversing ", index, ". root node <", root.header,
                     "> error occured! The process was aborted after ", time, " seconds."),
            LOGGING_INFO);
        return cpp::fail(std::make_shared<FileTraversorError>(result.error(), root));
      }

      if (result.value() == nullptr) {
        auto time = timer.End();
        LogError(FormatString("While traversing ", index, ". root node <",
                     "> unxpected internal error occured! The process was aborted after ", time, " seconds."),
            LOGGING_INFO);
        return cpp::fail(std::make_shared<FileTraversorError>(root) << std::make_shared<NullPointerError>());
      }

      traversedFile->Merge(result.value().get());
      index++;
    }

    auto time = timer.End();
    LogInfo(FormatString("Traversing finished successfully. The process took ", time, " seconds."), LOGGING_INFO);

    return traversedFile;
  }
}  // namespace vrml_proc::traversor::VrmlFileTraversor
