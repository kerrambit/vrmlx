#pragma once

#include <memory>

#include "ConversionContextActionMap.hpp"
#include "Error.hpp"
#include "FileTraversorError.hpp"
#include "FormatString.hpp"
#include "VrmlFileTraversorParameters.hpp"
#include "VrmlNodeTraversorParameters.hpp"
#include "Logger.hpp"
#include "ManualTimer.hpp"
#include "NullPointerError.hpp"
#include "VrmlFile.hpp"
#include "VrmlNodeManager.hpp"
#include "VrmlNodeTraversor.hpp"

#include "VrmlProcessingExport.hpp"

namespace vrml_proc::traversor::VrmlFileTraversor {

  template <typename ConversionContext>
  VRMLPROCESSING_API inline cpp::result<std::shared_ptr<ConversionContext>,
                                        std::shared_ptr<vrml_proc::core::error::Error>>
  Traverse(const VrmlFileTraversorParameters& context,
           const vrml_proc::action::ConversionContextActionMap<ConversionContext>& actionMap) {
    using namespace vrml_proc::core::logger;
    using namespace vrml_proc::core::utils;

    LogInfo("Traverse VRML file.", LOGGING_INFO);
    ManualTimer timer;
    timer.Start();

    std::shared_ptr<ConversionContext> traversedFile = std::make_shared<ConversionContext>();

    size_t index = 1;
    for (const auto& root : context.file) {
      vrml_proc::core::logger::LogInfo(
          vrml_proc::core::utils::FormatString("Found ", index, ". root node. It is type <", root.header, ">."),
          LOGGING_INFO);

      cpp::result<std::shared_ptr<ConversionContext>, std::shared_ptr<vrml_proc::core::error::Error>> result =
          vrml_proc::traversor::VrmlNodeTraversor::Traverse<ConversionContext>(
              vrml_proc::traversor::VrmlNodeTraversorParameters(
                  root, context.manager, false, vrml_proc::math::TransformationMatrix(), context.config),
              actionMap);

      if (result.has_error()) {
        auto time = timer.End();
        vrml_proc::core::logger::LogError(
            vrml_proc::core::utils::FormatString("While traversing ", index, ". root node <", root.header,
                                                 "> error occured! The process was aborted after ", time, " seconds."),
            LOGGING_INFO);
        auto fileError = std::make_shared<vrml_proc::traversor::error::FileTraversorError>(result.error(), root);
        return cpp::fail(fileError);
      }

      if (result.value() == nullptr) {
        auto time = timer.End();
        vrml_proc::core::logger::LogError(
            vrml_proc::core::utils::FormatString("While traversing ", index, ". root node <",
                                                 "> unxpected internal error occured! The process was aborted after ",
                                                 time, " seconds."),
            LOGGING_INFO);
        auto fileError = std::make_shared<vrml_proc::traversor::error::FileTraversorError>(root)
                         << std::make_shared<vrml_proc::core::error::NullPointerError>();
        return cpp::fail(fileError);
      }

      traversedFile->Merge(result.value().get());

      index++;
    }

    auto time = timer.End();
    LogInfo(FormatString("Traversing finished successfully. The process took ", time, " seconds."), LOGGING_INFO);

    return traversedFile;
  }
}  // namespace vrml_proc::traversor::VrmlFileTraversor
