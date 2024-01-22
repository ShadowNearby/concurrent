//
// Created by yanjingsheng on 2024/1/22.
//
#pragma once
#include <atomic>
#include <condition_variable>
#include <future>
#include <memory>
#include <queue>
#include <thread>
#include <vector>

namespace concurrent {
class ThreadPool {
  using Task = std::packaged_task<void()>;

 public:
  ThreadPool(const ThreadPool &) = delete;
  auto operator=(const ThreadPool &) -> ThreadPool & = delete;
  explicit ThreadPool(uint32_t max_thread = std::thread::hardware_concurrency());
  ~ThreadPool() { Shutdown(); }

  void Shutdown();
  void WaitAll();

  template <class F, class... Args>
  auto Commit(F &&f, Args &&...args) -> std::future<decltype(f(args...))>;

 private:
  std::atomic<bool> stop_;
  uint32_t max_thread_;
  std::atomic<uint32_t> available_thread_;
  std::condition_variable task_cv_;
  std::condition_variable empty_cv_;
  std::mutex mtx_;
  std::queue<Task> tasks_;
  std::vector<std::thread> threads_;
};

template <class F, class... Args>
auto ThreadPool::Commit(F &&f, Args &&...args) -> std::future<decltype(f(args...))> {
  using ReturnType = decltype(f(args...));
  if (stop_) {
    return std::future<ReturnType>();
  }
  std::shared_ptr<std::packaged_task<ReturnType()>> task =
      std::make_shared<std::packaged_task<ReturnType()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
  std::future<ReturnType> return_value = task->get_future();
  {
    std::lock_guard<std::mutex> lock(mtx_);
    tasks_.emplace([task]() -> void { (*task)(); });
  }
  task_cv_.notify_one();
  return return_value;
}

}  // namespace concurrent