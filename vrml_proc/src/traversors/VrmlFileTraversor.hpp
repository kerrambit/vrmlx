#pragma once

#include <functional>
#include <memory>

#include "ConversionContextable.hpp"
#include "ConversionContextActionMap.hpp"
#include "Error.hpp"
#include "FileTraversorError.hpp"
#include "FormatString.hpp"
#include "Logger.hpp"
#include "ManualTimer.hpp"
#include "NullPointerError.hpp"
#include "TraversorResult.hpp"
#include "VrmlFile.hpp"
#include "VrmlHeaders.hpp"
#include "VrmlNodeManager.hpp"
#include "VrmlNodeTraversor.hpp"
#include "VrmlNodeTraversorParameters.hpp"
#include "VrmlProcConfig.hpp"

namespace vrml_proc::traversor {
  /**
   * @brief Class responsible for VrmlFile traversal.
   * @tparam ConversionContext conversion context
   */
  template <vrml_proc::core::contract::ConversionContextable ConversionContext>
  class VrmlFileTraversor {
   public:
    /**
     * @brief Constructs new object,
     *
     * @param manager manager
     * @param config configuration file
     * @param actionMap map with stored actions
     * @param headersMap synonyms-to-canonical names mappings
     */
    VrmlFileTraversor(const vrml_proc::parser::service::VrmlNodeManager& manager,
        std::shared_ptr<vrml_proc::core::config::VrmlProcConfig> config,
        const vrml_proc::action::ConversionContextActionMap<ConversionContext>& actionMap,
        const vrml_proc::traversor::node_descriptor::VrmlHeaders& headersMap)
        : m_manager(manager), m_config(config), m_actionMap(actionMap), m_headersMap(headersMap) {}

    /**
     * @brief Traverses the VRML file (collection of root nodes). Function calls for each root VRML node a
     * VrmlNodeTraversor. Accumulated result is agggregated from all nodes and merged into ConversionContext.
     *
     * @tparam ConversionContext type of conversion context (result type of the traversal)
     * @param file file to traverse
     * @returns Conversion context or error
     */
    TraversorResult<ConversionContext> Traverse(const vrml_proc::parser::model::VrmlFile& file) {  //

      using vrml_proc::core::error::NullPointerError;
      using vrml_proc::math::TransformationMatrix;
      using vrml_proc::traversor::error::FileTraversorError;
      using namespace vrml_proc::core::logger;
      using namespace vrml_proc::core::utils;

      // ---------------------------------------------------

      LogInfo("Traverse VRML file.", LOGGING_INFO);

      ManualTimer timer;
      timer.Start();

      std::shared_ptr<ConversionContext> traversedFile = std::make_shared<ConversionContext>();
      auto traversor = VrmlNodeTraversor<ConversionContext>(m_manager, m_config, m_actionMap, m_headersMap);

      size_t index = 1;
      for (const auto& root : file) {
        LogInfo(FormatString("Found ", index, ". root node. It is type <", root.header, ">."), LOGGING_INFO);

        auto matrix = TransformationMatrix();
        auto result = traversor.Traverse(VrmlNodeTraversorParameters(std::cref(root), false, matrix));

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

   private:
    const vrml_proc::parser::service::VrmlNodeManager& m_manager;
    std::shared_ptr<vrml_proc::core::config::VrmlProcConfig> m_config;
    const vrml_proc::action::ConversionContextActionMap<ConversionContext>& m_actionMap;
    const vrml_proc::traversor::node_descriptor::VrmlHeaders& m_headersMap;
  };
}  // namespace vrml_proc::traversor
