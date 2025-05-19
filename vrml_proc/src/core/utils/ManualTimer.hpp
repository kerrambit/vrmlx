#pragma once

#include <chrono>

namespace vrml_proc::core::utils {

  /**
   * @brief The ManualTimer class represents a simple manual timer.
   */
  class ManualTimer {
   public:
    /**
     * @brief Constructs a ManualTimer.
     */
    ManualTimer() = default;

    /**
     * @brief Starts the timer.
     */
    void Start() {
      m_startTime = std::chrono::high_resolution_clock::now();
      m_lastResume = m_startTime;
    }

    /**
     * @brief Structure holding two values representing time elapsed since the last Resume (if there was not Resume
     * yet, the value is the same as Start) and time elapsed since the start of timer.
     */
    struct ResumeResult {
      double lastLapSpan;
      double totalTimeSpan;
    };

    /**
     * @brief Stops the timer and returns elapsed times.
     * @returns Structure containing time since last resume and total elapsed time.
     */
    ResumeResult Stop() {
      auto now = std::chrono::high_resolution_clock::now();
      return {std::chrono::duration<double>(now - m_lastResume).count(),
          std::chrono::duration<double>(now - m_startTime).count()};
    }

    /**
     * @brief Resumes the timer by updating last resume timestamp.
     */
    void Resume() { m_lastResume = std::chrono::high_resolution_clock::now(); }

    /**
     * @brief Ends the timer and resets its state.
     * @returns Total elapsed time in seconds.
     */
    double End() {
      auto timeSpan = std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - m_startTime).count();

      m_startTime = std::chrono::high_resolution_clock::time_point();
      m_lastResume = std::chrono::high_resolution_clock::time_point();

      return timeSpan;
    }

   private:
    std::chrono::high_resolution_clock::time_point m_startTime;
    std::chrono::high_resolution_clock::time_point m_lastResume;
  };
}  // namespace vrml_proc::core::utils
