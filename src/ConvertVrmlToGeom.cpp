#include "vrmlxpy.hpp"

#include <filesystem>
#include <iostream>
#include <string>

static inline void PrintHelp(const std::string& app) {
  std::cout << "There are two modes for this application. Either single conversion (default) or bulk conversion.\n";
  std::cout << "For single conversion, you do not specify the mode:\n";
  std::cout << "\t" << app << " <input_file> <output_file> <config_file>\n\n";
  std::cout << "For bulk conversion, you must specify the mode by adding option '--bulk':\n";
  std::cout << "\t" << app << " --bulk <input_folder> <output_folder>" << std::endl;
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

    vrmlxpy::PrintVersion();

    if (!vrmlxpy::ConvertVrmlToGeom(argv[1], argv[2], argv[3])) {
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
    if (entry.is_regular_file() && entry.path().filename() == "vrmlxpyConfig.json") {
      foundConfig = true;
      configFilePath = entry.path();
      break;
    }
  }

  if (!foundConfig) {
    std::cout << "Did not find necessary configuration file <vrmlxpyConfig.json> inside input folder!" << std::endl;
    return -1;
  }

  vrmlxpy::PrintVersion();

  if (std::ranges::distance(std::filesystem::directory_iterator(inputFolder), std::filesystem::directory_iterator{}) ==
      1) {
    std::cout << "\n>>> No files for conversion were found.\n" << std::endl;
  }

  std::string extension = vrmlxpy::GetExpectedOutputFileExtension(configFilePath.string());
  for (const auto& entry : std::filesystem::directory_iterator(inputFolder)) {
    if (entry.is_regular_file() && (entry.path().extension() == ".wrl" || entry.path().extension() == ".vrml")) {
      std::string filename = entry.path().filename().string();

      std::filesystem::path outputFile = outputFolder / entry.path().stem();
      outputFile.replace_extension(extension);

      vrmlxpy::ConvertVrmlToGeom(entry.path().string(), outputFile.string(), configFilePath.string());
      std::cout << std::endl;
    }
  }

  return 0;
}