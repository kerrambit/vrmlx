#pragma once

#include <stdexcept>
#include <vector>

#include "BaseConversionContext.hpp"

namespace vrml_proc::conversion_context {
  /**
   * @brief Templated child of `BaseConversionContext`. It is a specialized ConversionContext.
   * Its internal data format is std::vector<T>.
   */
  template <typename T>
  class VectorConversionContext : public BaseConversionContext {
   public:
    /**
     * @brief Default constructor.
     */
    VectorConversionContext() = default;
    /**
     * @brief Virtual default destructor.
     */
    virtual ~VectorConversionContext() = default;
    /**
     * @brief Gets const reference to the underlying vector of data.
     *
     * @returns const reference to std::vector<T>
     */
    inline const std::vector<T>& GetData() const { return m_data; }
    /**
     * @brief Adds new data entity.
     *
     * @param entity new data entity
     */
    inline void Add(T&& entity) { this->m_data.push_back(std::move(entity)); }
    /**
     * @brief Adds new data entity by a copy.
     *
     * @param entity new data entity
     */
    inline void CopyAndAdd(T entity) { this->m_data.push_back(entity); }
    /**
     * @brief Checks if there is any data entity present.
     *
     * @returns true if there are no data entities present otherwise false
     */
    inline bool IsEmpty() const { return m_data.empty(); }
    /**
     * @brief Overriden implemented interface method.
     * The `Merge` method moves internal data (from std::vector) from the passed pointer to the actual object.
     * Note that passed pointer' data storage aka std::vector will be empty and may not be safe to reuse.
     *
     * @param other pointer to another `Mergeable` object, must be of type VectorConversionContextl; if other is
     * nullptr, nothing will be merged
     *
     * @todo Merge should not throw exception but return Result type identifying the issue occured.
     * @todo Move it into seperated .cpp file.
     * @throws std::invalid_argument if the objects cannot be merged due to incompatibility.
     */
    void Merge(Mergeable* other) override {
      if (other == nullptr) {
        return;
      }
      VectorConversionContext* otherContext = dynamic_cast<VectorConversionContext<T>*>(other);
      if (otherContext) {
        if (otherContext->IsEmpty()) {
          return;
        }
        m_data.reserve(m_data.size() + otherContext->m_data.size());
        m_data.insert(m_data.end(), std::make_move_iterator(otherContext->m_data.begin()),
            std::make_move_iterator(otherContext->m_data.end()));
        otherContext->m_data.clear();
      } else {
        throw std::invalid_argument("Merge called with incompatible type!");
      }
    }

   protected:
    std::vector<T> m_data;

   private:
    VectorConversionContext(const VectorConversionContext&) = delete;
    VectorConversionContext& operator=(const VectorConversionContext&) = delete;
    VectorConversionContext(VectorConversionContext&&) = delete;
    VectorConversionContext& operator=(VectorConversionContext&&) = delete;
  };
}  // namespace vrml_proc::conversion_context
