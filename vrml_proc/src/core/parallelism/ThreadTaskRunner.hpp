#pragma once

#include <thread>
#include <mutex>

#include <taskflow/taskflow.hpp>

namespace vrml_proc::core::parallelism {
  /**
   * @brief Represents an object - kind of thread pool - which runs list of tasks parallely.
   *
   * @tparam TaskType type of the tasks
   * @tparam ResultType type of the result
   */
  template <typename TaskType, typename ResultType>
  class ThreadTaskRunner {
   public:
    /**
     * @brief Constructs an ThreadTaskRunner object.
     *
     * @param threads number of threads to use in the thread pool
     */
    explicit ThreadTaskRunner(unsigned int threads = std::thread::hardware_concurrency()) : m_threads(threads) {}

    /**
     * @brief Runs a list of tasks in parallel using `m_threads` threads and stores their results in the `results`
     * vector.
     *
     * Each task from `tasks[i]` is executed in parallel and its result is stored at `results[i]`.
     *
     * @param tasks vector of callable tasks, each must return a result compatible with `ResultType`
     * @param results output vector, where task results are stored; resized to match `tasks.size()`
     *
     * @note This function is thread-safe internally. Do not access `results` concurrently from other threads
     *       during execution.
     */
    void Run(const std::vector<TaskType>& tasks, std::vector<ResultType>& results) {
      tf::Executor executor{m_threads};
      tf::Taskflow taskflow;

      results.resize(tasks.size());
      std::mutex mutex;

      for (size_t i = 0; i < tasks.size(); ++i) {
        taskflow.emplace([&tasks, &results, i, &mutex]() {
          auto result = tasks[i]();
          {
            std::scoped_lock lock(mutex);
            results[i] = std::move(result);
          }
        });
      }

      executor.run(taskflow).wait();
    }

   private:
    unsigned int m_threads;
  };
}  // namespace vrml_proc::core::parallelism
