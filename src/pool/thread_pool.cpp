//
// Created by yanjingsheng on 2024/1/22.
//
#include "pool/thread_pool.h"
#include <fmt/core.h>

namespace concurrent {

ThreadPool::ThreadPool(uint32_t max_thread) : stop_(false), max_thread_(max_thread), available_thread_(max_thread) {
  for (uint32_t i = 0; i < max_thread_; ++i) {
    threads_.emplace_back([this]() -> void {
      while (!this->stop_) {
        Task task;
        {
          std::unique_lock<std::mutex> lock(mtx_);
          this->task_cv_.wait(lock, [this]() -> bool { return this->stop_ || !this->tasks_.empty(); });
          if (this->stop_) {
            return;
          }
          task = std::move(this->tasks_.front());
          this->tasks_.pop();
        }
        available_thread_--;
        task();
        available_thread_++;
        if (this->tasks_.empty() && this->available_thread_ == this->max_thread_) {
          this->empty_cv_.notify_one();
        }
      }
    });
  }
}

void ThreadPool::Shutdown() {
  stop_ = true;
  task_cv_.notify_all();
  for (auto &thread : threads_) {
    if (thread.joinable()) {
      thread.join();
    }
  }
}

void ThreadPool::WaitAll() {
  std::unique_lock<std::mutex> lock(mtx_);
  empty_cv_.wait(lock, [this]() -> bool { return tasks_.empty() && available_thread_ == max_thread_; });
  fmt::println("notify");
}

}  // namespace concurrent
