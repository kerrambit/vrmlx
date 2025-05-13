#pragma once

#include <algorithm>
#include <filesystem>
#include <future>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include <result.hpp>

#include <BufferView.hpp>
#include <ConversionContextActionMap.hpp>
#include <FormatString.hpp>
#include <JsonFileReader.hpp>
#include <Logger.hpp>
#include <MemoryMappedFileReader.hpp>
#include <ParserResult.hpp>
#include <VrmlFile.hpp>
#include <VrmlNodeManager.hpp>
#include <VrmlParser.hpp>

/**
 * @brief Reads error and logs the error into logging file.
 * @param error error to log
 */
static void LogError(std::shared_ptr<vrml_proc::core::error::Error> error) {
  vrml_proc::core::logger::LogUnformattedText("caught application error", error->GetMessage(),
                                              vrml_proc::core::logger::Level::Error, LOGGING_INFO);
}

/**
 * @brief Reads error message and logs it into logging file.
 * @param error error to log
 */
static void LogError(const std::string& error) {
  vrml_proc::core::logger::LogUnformattedText("caught application error", error, vrml_proc::core::logger::Level::Error,
                                              LOGGING_INFO);
}

/**
 * @brief Reads result and checks if it contains error, if yes, the error is logged.
 * @tparam T parameter of value inside result
 * @param result result to check
 */
template <typename T>
static void LogError(const cpp::result<std::shared_ptr<T>, std::shared_ptr<vrml_proc::core::error::Error>>& result) {
  if (result.has_error()) {
    LogError(result.error());
  }
}

/**
 * @brief Contains all data loaded from test configuration file.
 */
struct TestInfo {
  std::string baseInputPath;
  std::string baseOutputPath;
  std::string baseExpectedOutputPath;
  std::map<std::string, std::string> testFiles;
};

/**
 * @brief Static global variable holding the test information.
 */
static TestInfo g_testInfo;

/**
 * @brief Initialize testing.
 * @param filepath Path to test configuration file.
 * @return true if file was loaded succesfully, else false
 */
static bool InitTesting(const std::filesystem::path& filepath) {
  vrml_proc::core::io::JsonFileReader reader;
  auto json = reader.Read(std::filesystem::path(filepath));

  if (json.has_value()) {
    g_testInfo.baseInputPath = (json.value())["baseInputPath"];
    g_testInfo.baseOutputPath = (json.value())["baseOutputPath"];
    g_testInfo.baseExpectedOutputPath = (json.value())["baseExpectedOutputPath"];

    for (auto& element : (json.value())["testFiles"].items()) {
      g_testInfo.testFiles[element.key()] = element.value();
    }
    return true;
  } else {
    LogError(json.error());
    return false;
  }
}

/**
 * @brief Retrieves read-only reference to global test information structure.
 * @return  test information structure
 */
static const TestInfo& ReadTestInfo() { return g_testInfo; }

/**
 * @brief Generates file (with .stl extension) name based on test case name.
 */
#define GENERATE_TEST_OUTPUT_FILENAME(filepath)                          \
  std::string testName = Catch::getResultCapture().getCurrentTestName(); \
  std::replace(testName.begin(), testName.end(), ' ', '_');              \
  std::filesystem::path filepath = std::filesystem::path((testName + ".stl"))

/**
 * @brief Generates file (with .stl extension) name based on test case name. Add some extra name to the test -
 * for example one test case can have more exports in it.
 */
#define GENERATE_TEST_OUTPUT_FILENAME_EXTRA_NAME(filepath, extraName)    \
  std::string testName = Catch::getResultCapture().getCurrentTestName(); \
  std::replace(testName.begin(), testName.end(), ' ', '_');              \
  std::filesystem::path filepath = std::filesystem::path((testName + "_-_" + extraName + ".stl"))

/**
 * @brief Checks if the most inner error is equal to expected type.
 * @tparam ExpectedErrorType expected type
 * @param error error to check
 * @return true if the comparison was successfull, else false
 */
template <typename ExpectedErrorType>
static bool CheckInnermostError(std::shared_ptr<vrml_proc::core::error::Error> error) {
  return std::dynamic_pointer_cast<ExpectedErrorType>(vrml_proc::core::error::Error::GetInnermostError(error)) !=
         nullptr;
}

/**
 * @brief Retrieves the most inner error.
 * @tparam WantedErrorType type of error to retrieve
 * @param error error to get inner error from
 * @return shared pointer owning the inner error
 *
 * @warning nullptr can be returned if WantedErrorType is not the most inner error!
 */
template <typename WantedErrorType>
static std::shared_ptr<WantedErrorType> GetInnermostError(std::shared_ptr<vrml_proc::core::error::Error> error) {
  return std::dynamic_pointer_cast<WantedErrorType>(vrml_proc::core::error::Error::GetInnermostError(error));
}

/**
 * @brief Checks if two files have similiar size in bytes.
 * @param filepath1 first path to check
 * @param filepath2 second path to check
 * @param toleranceInBytes ± tolerance between sizes in bytes
 * @return true if the sizes are similiar else false
 */
bool HaveSimiliarSizes(const std::filesystem::path& filepath1, const std::filesystem::path& filepath2,
                       std::size_t toleranceInBytes) {
  auto size1 = std::filesystem::file_size(filepath1);
  auto size2 = std::filesystem::file_size(filepath2);
  bool result = std::abs(static_cast<std::ptrdiff_t>(size1) - static_cast<std::ptrdiff_t>(size2)) <=
                static_cast<std::ptrdiff_t>(toleranceInBytes);
  if (!result) {
    LogError(vrml_proc::core::utils::FormatString("Filesizes are not in the tolerance: ", size1, " B != ", size2,
                                                  " B (tolerance is +-", toleranceInBytes, " B)."));
  }
  return result;
}

/**
 * Checks if two binary files are the same.
 * @param filepath1 first path to check
 * @param filepath2 second path to check
 * @return true if the files are same
 */
bool AreBinaryFilesEqual(const std::filesystem::path& filepath1, const std::filesystem::path& filepath2) {
  std::ifstream f1(filepath1.string(), std::ios::binary);
  std::ifstream f2(filepath2.string(), std::ios::binary);

  f1.seekg(0, std::ios::end);
  f2.seekg(0, std::ios::end);
  if (f1.tellg() != f2.tellg()) {
    return false;
  }

  f1.seekg(0, std::ios::beg);
  f2.seekg(0, std::ios::beg);

  char c1, c2;
  while (f1.get(c1) && f2.get(c2)) {
    if (c1 != c2) {
      return false;
    }
  }
  return true;
}

/**
 * @brief Parses VRML text.
 * @param text text to parse
 * @param manager VRML node manager
 * @return result containing VRMLFile object or error
 */
static vrml_proc::parser::ParserResult<vrml_proc::parser::model::VrmlFile> ParseVrmlFile(
    std::string& text, vrml_proc::parser::service::VrmlNodeManager& manager) {
  vrml_proc::parser::VrmlParser parser(manager);
  return parser.Parse(vrml_proc::parser::BufferView(text.c_str(), text.c_str() + text.size()));
}

/**
 * @brief Parses VRML text.
 * @param filepath path to file, which will be read and parsed
 * @param manager VRML node manager
 * @return result containing VRMLFile object or error
 */
static vrml_proc::parser::ParserResult<vrml_proc::parser::model::VrmlFile> ParseVrmlFile(
    const std::filesystem::path& filepath, vrml_proc::parser::service::VrmlNodeManager& manager) {
  vrml_proc::core::io::MemoryMappedFileReader reader;
  auto readResult = reader.Read(filepath);
  if (readResult.has_error()) {
    LogError(readResult.error());
    return {};
  }

  vrml_proc::parser::VrmlParser parser(manager);
  vrml_proc::core::io::MemoryMappedFile file = readResult.value();
  return parser.Parse(vrml_proc::parser::BufferView(file.GetBegin(), file.GetEnd()));
}