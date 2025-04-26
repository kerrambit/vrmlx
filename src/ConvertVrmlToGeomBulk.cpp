#include "vrmlxpy.hpp"

#include <iostream>
#include <filesystem>

int main(int argc, char* argv[]) {
  if (argc != 3) {
    std::cout << "Usage: " << argv[0] << " <input_folder> <output_folder>" << std::endl;
    return -1;
  }

  std::filesystem::path inputFolder(argv[1]);
  std::filesystem::path outputFolder(argv[2]);

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

  std::string extension = vrmlxpy::GetExpectedOutputFileExtension(configFilePath.string());
  for (const auto& entry : std::filesystem::directory_iterator(inputFolder)) {
    if (entry.is_regular_file() && entry.path().extension() == ".wrl") {
      std::string filename = entry.path().filename().string();

      std::filesystem::path outputFile = outputFolder / entry.path().stem();
      outputFile.replace_extension(extension);

      vrmlxpy::ConvertVrmlToGeom(entry.path().string(), outputFile.string(), configFilePath.string());
      std::cout << std::endl;
    }
  }

  return 0;
}