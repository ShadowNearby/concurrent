//
// Created by yanjingsheng on 2024/1/22.
//
#pragma once
#include <atomic>
#include <memory>
#include <mutex>

namespace concurrent {
template <typename T>
class ConcurrentQueue {
 public:
  ConcurrentQueue();
  void Push(T value);
  std::shared_ptr<T> TryPop();
  std::shared_ptr<T> WaitAndPop();
  bool Empty();
  size_t Size();

 private:
  struct Node {
    std::shared_ptr<T> data{nullptr};
    std::unique_ptr<Node> next{nullptr};
  };
  std::unique_ptr<Node> head_;
  Node *tail_;
  std::mutex head_mtx_;
  std::mutex tail_mtx_;
  std::condition_variable cv_;
};
template <typename T>
ConcurrentQueue<T>::ConcurrentQueue() : head_(std::make_unique<Node>()), tail_(head_.get()) {}

template <typename T>
size_t ConcurrentQueue<T>::Size() {
  std::unique_lock<std::mutex> head_lock(head_mtx_);
  std::lock_guard<std::mutex> tail_lock(tail_mtx_);
  auto it = head_.get();
  size_t size = 0;
  while (it != tail_) {
    ++size;
    it = it->next.get();
  }
  return size;
}

template <typename T>
bool ConcurrentQueue<T>::Empty() {
  std::unique_lock<std::mutex> head_lock(head_mtx_);
  std::lock_guard<std::mutex> tail_lock(tail_mtx_);
  return head_.get() == tail_;
}

template <typename T>
std::shared_ptr<T> ConcurrentQueue<T>::WaitAndPop() {
  std::unique_lock<std::mutex> head_lock(head_mtx_);
  cv_.wait(head_lock, [this]() {
    std::lock_guard<std::mutex> tail_lock(tail_mtx_);
    return this->tail_ != head_.get();
  });
  std::unique_ptr<Node> old_head = std::move(head_);
  head_ = std::move(old_head->next);
  return old_head->data;
}

template <typename T>
std::shared_ptr<T> ConcurrentQueue<T>::TryPop() {
  std::unique_lock<std::mutex> head_lock(head_mtx_);
  std::lock_guard<std::mutex> tail_lock(tail_mtx_);
  if (tail_ == head_.get()) {
    return nullptr;
  }
  std::unique_ptr<Node> old_head = std::move(head_);
  head_ = std::move(old_head->next);
  return old_head->data;
}

template <typename T>
void ConcurrentQueue<T>::Push(T value) {
  auto new_data = std::make_shared<T>(std::move(value));
  auto new_tail_node = std::make_unique<Node>();
  auto *new_tail = new_tail_node.get();
  std::lock_guard<std::mutex> tail_lock(tail_mtx_);
  tail_->data = new_data;
  tail_->next = std::move(new_tail_node);
  tail_ = new_tail;
  cv_.notify_one();
}
}  // namespace concurrent
