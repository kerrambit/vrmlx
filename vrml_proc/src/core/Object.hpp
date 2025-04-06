#pragma once

#include <iostream>
#include <string>

namespace vrml_proc::core {
  /**
   * @brief Base abstract class in the class hiearchy.
   *
   * Should provide basic functionality as methods ToString(), GetHashCode(), Equals(), Copy(), Move() etc.
   */
  class Object {
   public:
    /**
     * @brief Virtual destructor.
     */
    virtual ~Object() = default;

    /**
     * @brief Returns string representation of a given object.
     *
     * @returns string representation
     */
    virtual std::string ToString() const = 0;

    /**
     * @brief Helper method to allow derived classes from Object to use '<<' operator.
     *
     * @param os stream
     * @param obj object to print
     */
    friend std::ostream& operator<<(std::ostream& os, const Object& obj) { return os << obj.ToString(); }

   protected:
    /**
     * @brief Publicly hiddent constrcutor.
     */
    Object() {}
  };
}  // namespace vrml_proc::core
