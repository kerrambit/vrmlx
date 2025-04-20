#pragma once

#include <cstdint>
#include <optional>

#include "VrmlUnits.hpp"

#if defined(BOOST_VERSION) && BOOST_VERSION >= 108700  // Class concurrent_node_map added in
                                                       // https://www.boost.org/users/history/version_1_87_0.html.
#include <boost/unordered/concurrent_node_map.hpp>
#define USE_BOOST_CONCURRENT_MAP
#else
#include <mutex>
#include <map>
#endif

namespace vrml_proc::parser::model::utils {

  class ExtractorCache {
   public:
    ExtractorCache() = default;

    void StoreFloat(std::uintptr_t key, vrml_proc::parser::float32_t value) {
#ifdef USE_BOOST_CONCURRENT_MAP
      m_cachedFloats.insert_or_assign(key, value);
#else
      std::lock_guard<std::mutex> lock(m_mutexFloats);
      m_cachedFloats[key] = value;
#endif
    }

    void StoreInt(std::uintptr_t key, int32_t value) {
#ifdef USE_BOOST_CONCURRENT_MAP
      m_cachedInts.insert_or_assign(key, value);
#else
      std::lock_guard<std::mutex> lock(m_mutexInts);
      m_cachedInts[key] = value;
#endif
    }

    std::optional<std::reference_wrapper<const vrml_proc::parser::float32_t>> GetFloat(std::uintptr_t key) {
      std::optional<std::reference_wrapper<const vrml_proc::parser::float32_t>> result;
#ifdef USE_BOOST_CONCURRENT_MAP
      m_cachedFloats.visit(key, [&](const auto& value) { result = std::make_optional(std::cref(value.second)); });
#else
      std::lock_guard<std::mutex> lock(m_mutexFloats);
      auto it = m_cachedFloats.find(key);
      if (it != m_cachedFloats.end()) {
        result = std::make_optional(std::cref(it->second));
      }
#endif
      return result;
    }

    std::optional<std::reference_wrapper<const int32_t>> GetInt(std::uintptr_t key) {
      std::optional<std::reference_wrapper<const int32_t>> result;
#ifdef USE_BOOST_CONCURRENT_MAP
      m_cachedInts.visit(key, [&](const auto& value) { result = std::make_optional(std::cref(value.second)); });
#else
      std::lock_guard<std::mutex> lock(m_mutexInts);
      auto it = m_cachedInts.find(key);
      if (it != m_cachedInts.end()) {
        result = std::make_optional(std::cref(it->second));
      }
#endif
      return result;
    }

   private:
#ifdef USE_BOOST_CONCURRENT_MAP
    boost::unordered::concurrent_node_map<std::uintptr_t, vrml_proc::parser::float32_t> m_cachedFloats;
    boost::unordered::concurrent_node_map<std::uintptr_t, int32_t> m_cachedInts;
#else
    std::map<std::uintptr_t, vrml_proc::parser::float32_t> m_cachedFloats;
    std::map<std::uintptr_t, int32_t> m_cachedInts;
    std::mutex m_mutexFloats;
    std::mutex m_mutexInts;
#endif
  };
}  // namespace vrml_proc::parser::model::utils
