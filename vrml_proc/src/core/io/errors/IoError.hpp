#pragma once

#include "Error.hpp"

namespace vrml_proc::core::io::error {

  /**
   * @brief Represents a general IO error.
   */
  class IoError : public vrml_proc::core::error::Error {
   protected:
    virtual std::string GetMessageInternal() const { return "[IoError]: IO error occured!\n"; }
  };

  /**
   * @brief Represents a conrete IO error meaning that filepath object is invalid or empty.
   */
  class EmptyFilePathError : public IoError {
   public:
    EmptyFilePathError(const std::string& filepath) : m_filepath(filepath) {}

   protected:
    std::string GetMessageInternal() const override {
      std::ostringstream oss;
      oss << "[EmptyFilePathError]: filepath <" << m_filepath << "> is empty and invalid!\n";
      return oss.str();
    }

   private:
    std::string m_filepath;
  };

  /**
   * @brief Represents a conrete IO error meaning that directory was not found.
   */
  class DirectoryNotFoundError : public IoError {
   public:
    DirectoryNotFoundError(const std::string& directoryPath) : m_directoryPath(directoryPath) {}

   protected:
    std::string GetMessageInternal() const override {
      std::ostringstream oss;
      oss << "[DirectoryNotFoundError]: directory <" << m_directoryPath << "> was not found!\n";
      return oss.str();
    }

   private:
    std::string m_directoryPath;
  };

  /**
   * @brief Represents a conrete IO error meaning that filepath was not found.
   */
  class FileNotFoundError : public IoError {
   public:
    FileNotFoundError(const std::string& filePath) : m_filePath(filePath) {}

   protected:
    std::string GetMessageInternal() const override {
      std::ostringstream oss;
      oss << "[FileNotFoundError]: file <" << m_filePath << "> was not found!\n";
      return oss.str();
    }

   private:
    std::string m_filePath;
  };

  /**
   * @brief Represents a general IO write error.
   */
  class GeneralWriteError : public IoError {
   public:
    GeneralWriteError(const std::string& filepath) : m_filepath(filepath) {}
    GeneralWriteError(const std::string& filepath, const std::string& details)
        : m_filepath(filepath), m_details(details) {}

   protected:
    std::string GetMessageInternal() const override {
      std::ostringstream oss;
      oss << "[GeneralWriteError]: when writing data into <" << m_filepath << ">, an unknown error occurred!";

      if (!m_details.empty()) {
        oss << " Details: <" << m_details << ">.";
      }

      oss << "\n";
      return oss.str();
    }

   private:
    std::string m_filepath;
    std::string m_details;
  };

  /**
   * @brief Represents a general IO read error.
   */
  class GeneralReadError : public IoError {
   public:
    GeneralReadError(const std::string& filepath) : m_filepath(filepath), m_details("") {}
    GeneralReadError(const std::string& filepath, const std::string& details)
        : m_filepath(filepath), m_details(details) {}

   protected:
    std::string GetMessageInternal() const override {
      std::ostringstream oss;
      oss << "[GeneralReadError]: when reading data from <" << m_filepath << ">, an unknown error occurred!";

      if (!m_details.empty()) {
        oss << " Details: <" << m_details << ">.";
      }

      oss << "\n";
      return oss.str();
    }

   private:
    std::string m_filepath;
    std::string m_details;
  };
}  // namespace vrml_proc::core::io::error
