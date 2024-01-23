//
// Created by yanjingsheng on 2024/1/23.
//
#include "stack/concurrent_stack.h"
#include <gtest/gtest.h>
#include <thread>
#include "fmt/core.h"

namespace concurrent {
TEST(ConcurrentStackTest, PushAndPop) {
  ConcurrentStack<int> stack;
  int num = 10;
  for (int i = 0; i < num; ++i) {
    stack.Push(i);
  }
  for (int i = num - 1; i >= 0; --i) {
    ASSERT_EQ(*stack.WaitAndPop(), i);
  }
  ASSERT_TRUE(stack.Empty());
}

TEST(ConcurrentStackTest, ConcurrentPushAndPop) {
  ConcurrentStack<int> stack;
  int num = 100;
  std::thread t1([&stack, num]() {
    for (int i = 0; i < num; ++i) {
      stack.Push(i);
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
  });
  std::thread t2([&stack, num]() {
    for (int i = 0; i < num; ++i) {
      ASSERT_EQ(*stack.WaitAndPop(), i);
    }
    ASSERT_TRUE(stack.Empty());
  });
  t1.join();
  t2.join();
}

TEST(ConcurrentStackTest, TryPop) {
  ConcurrentStack<int> stack;
  int num = 10;
  for (int i = 0; i < num; ++i) {
    int value = i;
    ASSERT_EQ(stack.TryPop(), nullptr);
    stack.Push(value);
    ASSERT_EQ(*stack.TryPop(), value);
  }
}
}  // namespace concurrent
