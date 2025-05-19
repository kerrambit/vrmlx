#pragma once

namespace vrml_proc::conversion_context {
  /**
   * @brief Interface defining a contract for the child classes.
   * Subclass, or rather an instance of the class, has an ability to "merge" other instance into itself.
   */
  class Mergeable {
   public:
    /**
     * @brief Virtual default destructor.
     */
    virtual ~Mergeable() = default;
    /**
     * @brief Pure virtual interface method. Merges the current object with another instance of the same type.
     * The `Merge` method defines how the state or data of one object should be combined with another.
     *
     * @param other Pointer to another `Mergeable` object, which should be of the same type as the current instance,
     * which is to be merged.
     *
     * @throws std::invalid_argument if the objects cannot be merged due to incompatibility.
     * @todo Merge should not throw exception but return Result type identifying the issue occured
     */
    virtual void Merge(Mergeable* other) = 0;
  };
}  // namespace vrml_proc::conversion_context
