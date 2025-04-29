#pragma once

#include <chrono>

namespace vrml_proc::core::utils {
  /**
   * @brief The ScopedTimer class represents a simple scoped timer.
   * @note The logic was inspired by the user 'jonnin' comment code snippet found on
   * https://cplusplus.com/forum/beginner/269195/.
   */
  class ScopedTimer {
   public:
    /**
     * @brief Constructs a ScopedTimer and starts the timer.
     * @param timerResult A reference to double where the timer result will be written to after ScopedTimer
     * destruction.
     */
    explicit ScopedTimer(double& timerResult) : m_elapsedTime(timerResult) {
      m_startTime = std::chrono::high_resolution_clock::now();
    }

    ~ScopedTimer() {
      auto timeSpan = std::chrono::duration_cast<std::chrono::duration<double>>(
          std::chrono::high_resolution_clock::now() - m_startTime);
      m_elapsedTime = timeSpan.count();
    }

    ScopedTimer(const ScopedTimer&) = delete;
    ScopedTimer& operator=(const ScopedTimer&) = delete;
    ScopedTimer(ScopedTimer&&) = delete;
    ScopedTimer& operator=(ScopedTimer&&) = delete;

   private:
    std::chrono::high_resolution_clock::time_point m_startTime;
    double& m_elapsedTime;
  };
}  // namespace vrml_proc::core::utils
