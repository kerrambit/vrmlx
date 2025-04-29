#pragma once

#include <memory>
#include <sstream>
#include <string>

namespace vrml_proc::core::error {
  /**
   * @brief Base class for Error.
   */
  class Error {
   public:
    /**
     * @brief Virtual destructor.
     */
    virtual ~Error() = default;

    /**
     * @brief Retrieves an error message formatted as a string describing error (and inner errors).
     *
     * @returns error message
     */
    virtual std::string GetMessage() const {  //

      std::ostringstream stream;
      stream << GetMessageInternal();

      if (m_innerError != nullptr) {
        m_innerError->m_indentation = m_indentation + 1;
        stream << std::string(2 * m_innerError->m_indentation, ' ') << m_innerError->GetMessage();
        m_innerError->m_indentation = 0;
      }

      return stream.str();
    }

    /**
     * @brief Sets an inner error.
     *
     * @param innerError inner error
     */
    void SetInnerError(std::shared_ptr<Error> innerError) { m_innerError = innerError; }

    /**
     * @brief Retrieves an inner error.
     *
     * @returns inner error
     */
    const std::shared_ptr<Error> GetInnerError() const { return m_innerError; }

    /**
     * @brief Operator <<, which sets an inner error;
     *
     * @param innerError inner error
     * @returns reference to itself
     */
    Error& operator<<(std::shared_ptr<Error> innerError) {
      SetInnerError(innerError);
      return *this;
    }

    /**
     * @brief Retrieves the innermost error in a nested error chain.
     *
     * This utility method traverses through the chain of errors to find
     * the deepest (most specific) error in the hierarchy.
     *
     * @param error The starting error to traverse.
     * @return A shared pointer to the innermost error, or nullptr if
     *         the input error is nullptr or error has no inner errors.
     */

    static std::shared_ptr<Error> GetInnermostError(std::shared_ptr<Error> error) {
      if (error == nullptr) {
        return nullptr;
      }

      std::shared_ptr<Error> curr = error;
      while (curr->GetInnerError() != nullptr) {
        curr = curr->GetInnerError();
      }

      return curr;
    }

   protected:
    /**
     * @brief Internal method which gets an error message.
     *
     * Used by GetMessage() internally. Each subclass should implement this method.
     *
     * @returns error message
     */
    virtual std::string GetMessageInternal() const = 0;

    /**
     * @brief Current indentation level.
     */
    size_t m_indentation = 0;

   private:
    std::shared_ptr<Error> m_innerError = nullptr;
  };

}  // namespace vrml_proc::core::error

/**
 * @brief Sets inner error (using shared_ptr).
 */
inline std::shared_ptr<vrml_proc::core::error::Error> operator<<(
    std::shared_ptr<vrml_proc::core::error::Error> left, std::shared_ptr<vrml_proc::core::error::Error> right) {
  if (left) {
    *left << right;
  }
  return left;
}