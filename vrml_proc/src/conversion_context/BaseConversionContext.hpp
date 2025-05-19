#pragma once

#include "Mergeable.hpp"

namespace vrml_proc::conversion_context {
  /**
   * @brief Base abstract class representing ConversionContext object. The ConversionContext object is what is created
   * when you traverse the AST and run your own actions/rules above it.
   *
   * @implements Mergeable
   */
  class BaseConversionContext : public Mergeable {
   public:
    /**
     * @brief Virtual default destructor.
     */
    virtual ~BaseConversionContext() = default;

   protected:
    BaseConversionContext() = default;

   private:
    BaseConversionContext(const BaseConversionContext&) = delete;
    BaseConversionContext& operator=(const BaseConversionContext&) = delete;
    BaseConversionContext(BaseConversionContext&&) = delete;
    BaseConversionContext& operator=(BaseConversionContext&&) = delete;
  };
}  // namespace vrml_proc::conversion_context
