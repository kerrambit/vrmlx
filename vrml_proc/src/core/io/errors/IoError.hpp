#pragma once

#include "Error.hpp"

namespace vrml_proc::core::io::error {

  class IoError : public vrml_proc::core::error::Error {
   protected:
    virtual std::string GetMessageInternal() const { return "[IoError]"; }
  };

  class EmptyFilePathError : public IoError {
   public:
    EmptyFilePathError(const std::string& filepath) : m_filepath(filepath) {}

   protected:
    std::string GetMessageInternal() const override {
      std::ostringstream oss;
      oss << IoError::GetMessageInternal() << "[EmptyFilePathError]: filepath <" << m_filepath
          << "> is empty and invalid!\n";
      return oss.str();
    }

   private:
    std::string m_filepath;
  };

  class DirectoryNotFoundError : public IoError {
   public:
    DirectoryNotFoundError(const std::string& directoryPath) : m_directoryPath(directoryPath) {}

   protected:
    std::string GetMessageInternal() const override {
      std::ostringstream oss;
      oss << IoError::GetMessageInternal() << "[DirectoryNotFoundError]: directory <" << m_directoryPath
          << "> was not found!\n";
      return oss.str();
    }

   private:
    std::string m_directoryPath;
  };

  class FileNotFoundError : public IoError {
   public:
    FileNotFoundError(const std::string& filePath) : m_filePath(filePath) {}

   protected:
    std::string GetMessageInternal() const override {
      std::ostringstream oss;
      oss << IoError::GetMessageInternal() << "[FileNotFoundError]: file <" << m_filePath << "> was not found!\n";
      return oss.str();
    }

   private:
    std::string m_filePath;
  };

  class GeneralWriteError : public IoError {
   public:
    GeneralWriteError(const std::string& filepath) : m_filepath(filepath) {}

   protected:
    std::string GetMessageInternal() const override {
      std::ostringstream oss;
      oss << IoError::GetMessageInternal() << "[GeneralWriteError]: when writing data into <" << m_filepath
          << ">, an unknown error occured!\n";
      return oss.str();
    }

   private:
    std::string m_filepath;
  };

  class GeneralReadError : public IoError {
   public:
    GeneralReadError(const std::string& filepath) : m_filepath(filepath), m_details("") {}
    GeneralReadError(const std::string& filepath, const std::string& details)
        : m_filepath(filepath), m_details(details) {}

   protected:
    std::string GetMessageInternal() const override {
      std::ostringstream oss;
      oss << IoError::GetMessageInternal() << "[GeneralReadError]: when reading data from <" << m_filepath
          << ">, an unknown error occurred!";

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
