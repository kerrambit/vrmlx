#pragma once

#include <memory>

#include "ConversionContextActionMap.hpp"
#include "Error.hpp"
#include "FileTraversorError.hpp"
#include "FormatString.hpp"
#include "Logger.hpp"
#include "ManualTimer.hpp"
#include "NullPointerError.hpp"
#include "TraversorResult.hpp"
#include "VrmlFile.hpp"
#include "VrmlFileTraversorParameters.hpp"
#include "VrmlNodeManager.hpp"
#include "VrmlNodeTraversor.hpp"
#include "VrmlNodeTraversorParameters.hpp"
#include "ConversionContextable.hpp"

namespace vrml_proc::traversor::VrmlFileTraversor {
  /**
   * @brief Traverses the VRML file (connection of root nodes). Function calls for each root VRML node a
   * VrmlNodeTraversor. Accumulated result is agggregated from all nodes into ConversionContext.
   *
   * @tparam ConversionContext type of conversion context (result type of the traversal)
   * @param params parameters for the traversal
   * @param actionMap map with defined actions
   */
  template <ConversionContextable ConversionContext>
  inline TraversorResult<ConversionContext> Traverse(const VrmlFileTraversorParameters& params,
      const vrml_proc::action::ConversionContextActionMap<ConversionContext>& actionMap) {  //

    using vrml_proc::core::error::NullPointerError;
    using vrml_proc::math::TransformationMatrix;
    using vrml_proc::traversor::error::FileTraversorError;
    using namespace vrml_proc::core::logger;
    using namespace vrml_proc::core::utils;

    LogInfo("Traverse VRML file.", LOGGING_INFO);

    ManualTimer timer;
    timer.Start();

    std::shared_ptr<ConversionContext> traversedFile = std::make_shared<ConversionContext>();

    size_t index = 1;
    for (const auto& root : params.file) {
      LogInfo(FormatString("Found ", index, ". root node. It is type <", root.header, ">."), LOGGING_INFO);

      auto result = vrml_proc::traversor::VrmlNodeTraversor::Traverse<ConversionContext>(
          VrmlNodeTraversorParameters(root, params.manager, false, TransformationMatrix(), params.config), actionMap);

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
