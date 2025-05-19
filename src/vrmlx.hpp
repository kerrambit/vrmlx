#pragma once

#include <string>

namespace vrmlx {
  /**
   * @brief Prints current vrmlx version.
   */
  void PrintVersion();

  /**
   * @brief Retrieves an output file name extension from configuration file.
   *
   * @param configFilename path to the config file
   * @returns string representing an extension of the output file
   */
  std::string GetExpectedOutputFileExtension(const std::string& configFilename);

  /**
   * @brief Converts a VRML file (`inputFilename`) into a geometry representation (given by configuration file) and
   * writes result in `outputFileName`.
   *
   * @param inputFilename input VRML file
   * @param outputFilename output file
   * @param configFilename path to configuration file
   *
   * @returns true if conversion was successfull, otherwise false
   */
  bool ConvertVrmlToGeom(
      const std::string& inputFilename, const std::string& outputFilename, const std::string& configFilename);
}  // namespace vrmlx
