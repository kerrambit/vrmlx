#include "vrmlxpy.hpp"
#include "vrmlxpy_logo.hpp"

#include <memory>
#include <filesystem>
#include <future>
#include <iostream>
#include <string>
#include <vector>
#include <thread>

#include <BufferView.hpp>
#include <CalculatorResult.hpp>
#include <Logger.hpp>
#include <MemoryMappedFileReader.hpp>
#include <Mesh.hpp>
#include <MeshTaskConversionContext.hpp>
#include <ToGeomActionMap.hpp>
#include <StlFileWriter.hpp>
#include <ObjFileWriter.hpp>
#include <PlyFileWriter.hpp>
#include <VrmlFileTraversor.hpp>
#include <VrmlNodeManager.hpp>
#include <VrmlParser.hpp>
#include <VrmlProcConfig.hpp>
#include <ToGeomConfig.hpp>
#include <ExportFormats.hpp>
#include <FileWriter.hpp>
#include <ManualTimer.hpp>
#include <ThreadTaskRunner.hpp>
#include <MeshTask.hpp>
#include <MeshSimplificator.hpp>

static unsigned int g_task = 1;

static void PrintApplicationError(std::shared_ptr<vrml_proc::core::error::Error> error) {
  std::cout << "Caught an application error:\n" << error->GetMessage() << std::endl;
  g_task = 1;
}

static void PrintProgressInformation(const std::string& information) {
  std::cout << "[" << g_task << "/6]: " << information << std::endl;
  g_task++;
  if (g_task > 6) {
    g_task = 1;
  }
}

static void PrintDefaultLoggingMessage() {
  using namespace std::filesystem;
  std::cout << "[Warning]: log file was created on the current path (" << current_path()
            << "). If there was a log directory specified inside "
               "the configuration file, do not look for it on this path as loading of configuration file failed!\n"
            << std::endl;
}

static void PrintInvalidSubmeshMessage(to_geom::calculator::CalculatorResult meshResult) {
  std::cout << "Encountered an invalid submesh:\n" << meshResult.error()->GetMessage() << std::endl;
}

namespace vrmlxpy {

  void PrintVersion() {
    std::cout << Logo << R"(
            vrmlxpy  (version 1.0)
            vrmlproc (version 1.0)
            togeom   (version 1.0))"
              << std::endl;
  }

  std::string GetExpectedOutputFileExtension(const std::string& configFilename) {  //

    using namespace to_geom::core::io;
    using namespace to_geom::core::config;

    ToGeomConfig config;
    auto configResult = config.Load(configFilename);
    if (configResult.has_error()) {
      return "txt";
    }

    switch (config.exportFormat) {
      case ExportFormat::Stl:
        return "stl";
      case ExportFormat::Ply:
        return "ply";
      case ExportFormat::Obj:
        return "obj";
      default:
        "txt";
    }

    return "txt";
  }

  bool ConvertVrmlToGeom(
      const std::string& inputFilename, const std::string& outputFilename, const std::string& configFilename) {  //

    using namespace std::filesystem;
    using namespace to_geom::core::config;
    using namespace vrml_proc::core::logger;
    using namespace vrml_proc::parser;
    using namespace vrml_proc::traversor::VrmlFileTraversor;
    using namespace to_geom::conversion_context;
    using namespace vrml_proc::core::io;
    using namespace to_geom::core::io;
    using namespace vrml_proc::core::utils;
    using namespace to_geom::core;
    using namespace to_geom::calculator;

    // -------------------------------------------------------------------------------------------------------------

    std::cout << "\n>>> Converting VRML file to geometry format..." << std::endl;

    // -------------------------------------------------------------------------------------------------------------

    std::shared_ptr<ToGeomConfig> config = std::make_shared<ToGeomConfig>();
    auto configResult = config->Load(configFilename);
    if (configResult.has_error()) {
      PrintApplicationError(configResult.error());
      // Initialize logging on the current directory and push all saved messages into it, since configuratation file
      // could not be loaded.
      InitLogging(current_path().string(), "vrmlxpy");
      PrintDefaultLoggingMessage();
      return false;
    }

    InitLogging(config->logFileDirectory, config->logFileName);
    PrintProgressInformation(
        FormatString("configuration file <", path(configFilename).string(), "> was succesfully read."));

    // -------------------------------------------------------------------------------------------------------------

    MemoryMappedFileReader reader;
    auto readResult = reader.Read(path(inputFilename));
    if (readResult.has_error()) {
      PrintApplicationError(readResult.error());
      return false;
    }

    PrintProgressInformation(FormatString("file <", path(inputFilename).string(), "> was succesfully read."));

    // -------------------------------------------------------------------------------------------------------------

    VrmlNodeManager manager;
    VrmlParser parser(manager);
    auto parseResult = parser.Parse(BufferView(readResult.value().GetBegin(), readResult.value().GetEnd()));
    if (parseResult.has_error()) {
      PrintApplicationError(parseResult.error());
      return false;
    }

    PrintProgressInformation(FormatString("file <", path(inputFilename).string(), "> was succesfully parsed."));

    // -------------------------------------------------------------------------------------------------------------

    auto convertResult = Traverse<MeshTaskConversionContext>({parseResult.value(), manager, config}, GetActionMap());
    if (convertResult.has_error()) {
      PrintApplicationError(convertResult.error());
      return false;
    }

    PrintProgressInformation(FormatString("file <", path(inputFilename).string(), "> was succesfully traversed."));

    // -------------------------------------------------------------------------------------------------------------

    LogInfo(
        FormatString("Generation of total ", convertResult.value()->GetData().size(), " meshes begins."), LOGGING_INFO);

    vrml_proc::core::utils::ManualTimer timer;
    timer.Start();

    std::vector<CalculatorResult> submeshesResults;
    submeshesResults.reserve(convertResult.value()->GetData().size());

    if (!config->parallelismSettings.active) {
      for (const auto& task : convertResult.value()->GetData()) {
        submeshesResults.emplace_back(task());
      }
    } else {
      unsigned int availableThreadsNumber = std::thread::hardware_concurrency();
      if (availableThreadsNumber > config->parallelismSettings.threadsNumberLimit) {
        availableThreadsNumber = config->parallelismSettings.threadsNumberLimit;
      }

      LogInfo(
          FormatString("Generation will be parallely computed on ", availableThreadsNumber, " threads."), LOGGING_INFO);

      auto runner = vrml_proc::core::parallelism::ThreadTaskRunner<MeshTask, CalculatorResult>(availableThreadsNumber);
      runner.Run(convertResult.value()->GetData(), submeshesResults);
    }

    to_geom::core::Mesh mesh;
    for (auto& submeshResult : submeshesResults) {
      if (submeshResult.has_value()) {
        auto submesh = submeshResult.value();
        mesh.join(*submesh);
        submesh.reset();
      } else {
        PrintInvalidSubmeshMessage(submeshResult);
      }
    }

    double time = timer.End();
    LogInfo(
        FormatString("Generation and merging of meshes ended. The generation took ", time, " seconds."), LOGGING_INFO);

    // -------------------------------------------------------------------------------------------------------------

    if (config->meshSimplificationSettings.active) {
      to_geom::calculator::MeshSimplificator::SimplifyMesh(
          mesh, config->meshSimplificationSettings.percentageOfAllEdgesToSimplify.GetComplement());
    }

    PrintProgressInformation(FormatString(
        "mesh was succesfully generated", ((config->meshSimplificationSettings.active) ? " and simplified." : ".")));

    // -------------------------------------------------------------------------------------------------------------

    std::unique_ptr<FileWriter<to_geom::core::Mesh>> writer;
    switch (config->exportFormat) {
      case ExportFormat::Stl:
        writer = std::make_unique<StlFileWriter>(config->exportFormatOptions.binary);
        break;
      case ExportFormat::Ply:
        writer = std::make_unique<PlyFileWriter>(config->exportFormatOptions.binary);
        break;
      case ExportFormat::Obj:
        writer = std::make_unique<ObjFileWriter>();
        break;
      default:
        writer = std::make_unique<StlFileWriter>(config->exportFormatOptions.binary);
        break;
    }

    auto writeResult = writer->Write(path(outputFilename), mesh);
    if (writeResult.has_error()) {
      PrintApplicationError(writeResult.error());
      return false;
    }

    PrintProgressInformation(FormatString("file <", path(outputFilename).string(), "> was succesfully written."));

    // -------------------------------------------------------------------------------------------------------------

    std::cout << ">>> Conversion of VRML file to geometry format finished succesfully.\n" << std::endl;
    return true;
  }
}  // namespace vrmlxpy