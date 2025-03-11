#pragma once

#include <memory>
#include <sstream>
#include <string>

namespace vrml_proc {
  namespace core {
    namespace error {

      class Error {
       public:
        virtual ~Error() = default;

        virtual std::string GetMessage() const {
          std::ostringstream stream;
          stream << GetMessageInternal();

          if (m_innerError != nullptr) {
            m_innerError->m_indentation = m_indentation + 1;
            stream << std::string(2 * m_innerError->m_indentation, ' ') << m_innerError->GetMessage();
            m_innerError->m_indentation = 0;
          }

          return stream.str();
        }
        void SetInnerError(std::shared_ptr<Error> inner) { m_innerError = inner; }
        const std::shared_ptr<Error> GetInnerError() const { return m_innerError; }
        Error& operator<<(std::shared_ptr<Error> innerError) {
          SetInnerError(innerError);
          return *this;
        }
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
        virtual std::string GetMessageInternal() const = 0;
        size_t m_indentation = 0;

       private:
        std::shared_ptr<Error> m_innerError = nullptr;
      };

    }  // namespace error
  }    // namespace core
}  // namespace vrml_proc

inline std::shared_ptr<vrml_proc::core::error::Error> operator<<(std::shared_ptr<vrml_proc::core::error::Error> left,
                                                                 std::shared_ptr<vrml_proc::core::error::Error> right) {
  if (left) {
    *left << right;
  }
  return left;
}