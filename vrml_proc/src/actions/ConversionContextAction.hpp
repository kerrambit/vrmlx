#pragma once

#include <memory>

#include "ConversionContextable.hpp"

namespace vrml_proc::action {
  /**
   * @brief Base abstract class representing Action object. The Action defines an interface
   * with method called Execute() which returns a shared pointer owning a ConversionContext object.
   * The Action object is encapsulation of a "program" or a "function", which returns a ConversionContext object.
   */
  template <vrml_proc::core::contract::ConversionContextable ConversionContext>
  class ConversionContextAction {
   public:
    /**
     * @brief Virtual default destructor.
     */
    virtual ~ConversionContextAction() = default;
    /**
     * @brief Pure virtual method which executes on the given Action object.
     *
     * @returns Shared pointer containing the ConversionContext object (of type base class BaseConversionContext).
     */
    virtual std::shared_ptr<ConversionContext> Execute() = 0;

   protected:
    ConversionContextAction() = default;

   private:
    ConversionContextAction(const ConversionContextAction&) = delete;
    ConversionContextAction& operator=(const ConversionContextAction&) = delete;
    ConversionContextAction(ConversionContextAction&&) = delete;
    ConversionContextAction& operator=(ConversionContextAction&&) = delete;
  };
}  // namespace vrml_proc::action
