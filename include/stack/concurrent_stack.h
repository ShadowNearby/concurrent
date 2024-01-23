//
// Created by yanjingsheng on 2024/1/23.
//

#pragma once
#include <atomic>
#include <mutex>
#include <optional>
#include <stack>

namespace concurrent {

template <typename T>
class ConcurrentStack {
 public:
  void Push(T value);
  std::shared_ptr<T> WaitAndPop();
  std::shared_ptr<T> TryPop();
  bool Empty();
  size_t Size();

 private:
  std::stack<std::shared_ptr<T>> data_;
  mutable std::mutex mtx_;
  std::condition_variable cv_;
};

template <typename T>
size_t ConcurrentStack<T>::Size() {
  std::lock_guard<std::mutex> lock(mtx_);
  return data_.size();
}

template <typename T>
bool ConcurrentStack<T>::Empty() {
  std::lock_guard<std::mutex> lock(mtx_);
  return data_.empty();
}

template <typename T>
void ConcurrentStack<T>::Push(T value) {
  std::lock_guard<std::mutex> lock(mtx_);
  data_.push(std::make_unique<T>(std::move(value)));
  cv_.notify_one();
}

template <typename T>
std::shared_ptr<T> ConcurrentStack<T>::WaitAndPop() {
  std::unique_lock<std::mutex> lock(mtx_);
  cv_.wait(lock, [this]() -> bool { return !this->data_.empty(); });
  auto value = data_.top();
  data_.pop();
  return value;
}

template <typename T>
std::shared_ptr<T> ConcurrentStack<T>::TryPop() {
  std::lock_guard<std::mutex> lock(mtx_);
  if (data_.empty()) {
    return nullptr;
  }
  auto value = data_.top();
  data_.pop();
  return value;
}
}  // namespace concurrent