#pragma once

#include <algorithm>
#include <filesystem>
#include <future>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include <result.hpp>

#include <BoxAction.hpp>
#include <BufferView.hpp>
#include <CalculatorResult.hpp>
#include <ConversionContextActionMap.hpp>
#include <FormatString.hpp>
#include <Logger.hpp>
#include <Mesh.hpp>
#include <MeshTaskConversionContext.hpp>
#include <ParserResult.hpp>
#include <StlActionMap.hpp>
#include <StlFileWriter.hpp>
#include <VrmlFile.hpp>
#include <VrmlFileTraversor.hpp>
#include <VrmlNodeManager.hpp>

#include "./TestCommon.hpp"

/**
 * @brief Traverses the VRML tree and creates conversion context.
 * @param parseResult VRML tree
 * @param manager VRML node manager
 * @param expectedConversionContextSize traversor creates vector of individual submeshes, this is expected count of them
 * @return false if traversing fails or if the expected count is not met, otherwise true
 */
static bool TraverseVrmlFileToMeshTask(vrml_proc::parser::ParserResult<vrml_proc::parser::VrmlFile> parseResult,
                                       const vrml_proc::parser::VrmlNodeManager& manager,
                                       size_t expectedConversionContextSize) {

    auto traversorResult = vrml_proc::traversor::VrmlFileTraversor::Traverse<to_stl::conversion_context::MeshTaskConversionContext>({ parseResult.value(), manager }, to_stl::conversion_context::CreateActionMap());
    if (traversorResult.has_error()) {
        LogError(traversorResult.error());
        return false;
    }
    else {
        if (traversorResult.value()->GetData().size() != expectedConversionContextSize) {
            LogError(vrml_proc::core::utils::FormatString("Size of traversor result is not the same as expected: ", traversorResult.value()->GetData().size(), " != ", expectedConversionContextSize, "."));
            return false;
        }
        return true;
    }
}

/**
 * @brief Traverses the VRML tree and creates conversion context. All tasks are then run in parallel, and result mesh is exported into file.
 * @param parseResult VRML tree
 * @param manager VRML node manager
 * @param expectedConversionContextSize traversor creates vector of individual submeshes, this is expected count of them
 * @param outputFilepath output file path
 * @param expectedSubmeshesCount all tasks are run, this is a number representing how many submeshes from tasks (expectedConversionContextSize) should be converted
 * @return false if traversing fails, if the expected count of taksks/submeshes is not met or if export fails, otherwise true
 */
static bool TraverseVrmlFileToMeshTask(vrml_proc::parser::ParserResult<vrml_proc::parser::VrmlFile> parseResult,
                                       const vrml_proc::parser::VrmlNodeManager& manager,
                                       size_t expectedConversionContextSize,
                                       const std::filesystem::path& outputFilepath,
                                       size_t expectedSubmeshesCount) {

    auto traversorResult = vrml_proc::traversor::VrmlFileTraversor::Traverse<to_stl::conversion_context::MeshTaskConversionContext>({ parseResult.value(), manager }, to_stl::conversion_context::CreateActionMap());
    if (traversorResult.has_error()) {
        LogError(traversorResult.error());
        return false;
    }

    if (traversorResult.value()->GetData().size() != expectedConversionContextSize) {
        LogError(vrml_proc::core::utils::FormatString("Size of traversor result is not the same as expected: ", traversorResult.value()->GetData().size(), " != ", expectedConversionContextSize, "."));
        return false;
    }

    std::vector<std::future<to_stl::calculator::CalculatorResult>> results;
    for (const auto& task : traversorResult.value()->GetData()) {
        if (task) {
            results.emplace_back(std::async(std::launch::async, task));
        }
    }

    to_stl::core::Mesh mesh;
    size_t currentSubmesh = 0;
    for (auto& future : results) {
        auto meshResult = future.get();
        if (meshResult.has_value()) {
            mesh.join(*(meshResult.value()));
            currentSubmesh++;
        }
        else {
            LogError(meshResult.error());
        }
    };

    if (currentSubmesh != expectedSubmeshesCount) {
        LogError(vrml_proc::core::utils::FormatString("Count of submeshes is not equal to expected count: ", currentSubmesh, " != ", expectedSubmeshesCount, "."));
        return false;
    }

    to_stl::core::io::StlFileWriter writer;
    auto writeResult = writer.Write(std::filesystem::path(outputFilepath), mesh);
    if (writeResult.has_error()) {
        LogError(writeResult.error());
        return false;
    }

    return true;
}

/**
 * @brief Traverses the VRML tree.
 * @param parseResult VRML tree
 * @param manager VRML node manager
 * @return false if traversing fails, otherwise true
 */
static bool TraverseVrmlFileToMeshTask(vrml_proc::parser::ParserResult<vrml_proc::parser::VrmlFile> parseResult,
                                       const vrml_proc::parser::VrmlNodeManager& manager) {
    auto traversorResult = vrml_proc::traversor::VrmlFileTraversor::Traverse<to_stl::conversion_context::MeshTaskConversionContext>({ parseResult.value(), manager }, to_stl::conversion_context::CreateActionMap());
    if (traversorResult.has_error()) {
        LogError(traversorResult.error());
        return false;
    }
    else return true;
}
