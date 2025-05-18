#include "vrmlx.hpp"

#include <filesystem>
#include <iostream>
#include <string>

static inline void PrintHelp(const std::string& app) {
  std::cout << "\nThere are two modes for this application. Either single conversion (default) or bulk conversion.\n";
  std::cout << "For single conversion, you do not specify the mode:\n";
  std::cout << "\t" << app << " <input_file> <output_file> <config_file>\n\n";
  std::cout << "For bulk conversion, you must specify the mode by adding option '--bulk':\n";
  std::cout << "\t" << app << " --bulk <input_folder> <output_folder>\n";

  std::cout << "\n-------------------------------------------------------------------------------------\n\nThe "
               "<config_file> is a JSON file with the "
               "following options (all have default values):\n";
  std::cout << "  \"ignoreUnknownNode\": Whether to ignore unrecognized VRML nodes (default: false).\n";
  std::cout << "  \"logFileName\": The base name of the log file (default: \"vrmlproc\").\n";
  std::cout << "  \"logFileDirectory\": Directory where logs will be written (default: \".\").\n";
  std::cout
      << "  \"synonymsFile\": Path to a JSON file defining node name synonyms (default: file './synonymsFile.json').\n";

  std::cout << "  \"exportFormat\":\n";
  std::cout
      << "    \"format\": The output format. Possible values are \"stl\", \"ply\", and \"obj\" (default: \"stl\").\n";
  std::cout << "    \"options\":\n";
  std::cout << "      \"binary\": Whether to export binary STL instead of ASCII (default: true).\n";

  std::cout << "  \"parallelismSettings\":\n";
  std::cout << "    \"active\": Enable parallel execution of conversion tasks (default: true).\n";
  std::cout
      << "    \"threadsNumberLimit\": Maximum number of threads to use (default: maximal number of threads on your "
         "system).\n";

  std::cout << "  \"meshSimplification\":\n";
  std::cout << "    \"active\": Whether to simplify meshes after conversion (default: false).\n";
  std::cout << "    \"percentageOfAllEdgesToSimplify\": Percentage of edges to simplify if enabled (default: 50).\n";

  std::cout << "  \"IFSSettings\":\n";
  std::cout << "    \"checkRange\": Enable range checking for IndexedFaceSet indices (default: true).\n\n";

  std::cout << "Note that <config_file> must be in <input_folder> for bulk conversion!\n" << std::endl;
}

int main(int argc, char* argv[]) {
  if (argc != 4) {
    PrintHelp(argv[0]);
    return -1;
  }

  // -------------------------------------------------------------------------------------------------------------------

  bool bulkConversion = (std::string(argv[1]) == "--bulk");

  if (!bulkConversion) {
    if (!std::filesystem::is_regular_file(argv[1])) {
      std::cout << "Input file does not exist or it is not a valid file!" << std::endl;
      return -1;
    }

    if (!std::filesystem::is_directory(std::filesystem::path(argv[2]).parent_path())) {
      std::cout << "Directory for output file does not exist or it is not a valid directory!" << std::endl;
      return -1;
    }

    if (!std::filesystem::is_regular_file(argv[3])) {
      std::cout << "Configuration file does not exist or it is not a valid file!" << std::endl;
      return -1;
    }

    vrmlx::PrintVersion();

    if (!vrmlx::ConvertVrmlToGeom(argv[1], argv[2], argv[3])) {
      return -1;
    }
    return 0;
  }

  std::filesystem::path inputFolder(argv[2]);
  std::filesystem::path outputFolder(argv[3]);

  if (!std::filesystem::is_directory(inputFolder)) {
    std::cout << "Input folder is not a valid directory!" << std::endl;
    return -1;
  }

  if (!std::filesystem::is_directory(outputFolder)) {
    std::cout << "Output folder is not a valid directory!" << std::endl;
    return -1;
  }

  bool foundConfig = false;
  std::filesystem::path configFilePath;
  for (const auto& entry : std::filesystem::directory_iterator(inputFolder)) {
    if (entry.is_regular_file() && entry.path().filename() == "vrmlxConfig.json") {
      foundConfig = true;
      configFilePath = entry.path();
      break;
    }
  }

  if (!foundConfig) {
    std::cout << "Did not find necessary configuration file <vrmlxConfig.json> inside input folder!" << std::endl;
    return -1;
  }

  vrmlx::PrintVersion();

  if (std::ranges::distance(std::filesystem::directory_iterator(inputFolder), std::filesystem::directory_iterator{}) ==
      1) {
    std::cout << "\n>>> No files for conversion were found.\n" << std::endl;
  }

  std::string extension = vrmlx::GetExpectedOutputFileExtension(configFilePath.string());
  for (const auto& entry : std::filesystem::directory_iterator(inputFolder)) {
    if (entry.is_regular_file() && (entry.path().extension() == ".wrl" || entry.path().extension() == ".vrml")) {
      std::string filename = entry.path().filename().string();

      std::filesystem::path outputFile = outputFolder / entry.path().stem();
      outputFile.replace_extension(extension);

      vrmlx::ConvertVrmlToGeom(entry.path().string(), outputFile.string(), configFilePath.string());
      std::cout << std::endl;
    }
  }

  return 0;
}