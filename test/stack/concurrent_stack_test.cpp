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

TEST(ConcurrentStackTest, ConcurrentPush) {
  ConcurrentStack<int> stack;
  int num = 10000;
  std::thread t1([&stack, num]() {
    for (int i = 0; i < num; ++i) {
      stack.Push(i);
    }
  });
  std::thread t2([&stack, num]() {
    for (int i = num; i < num * 2; ++i) {
      stack.Push(i);
    }
  });
  t1.join();
  t2.join();
  ASSERT_EQ(stack.Size(), num * 2);
}

TEST(ConcurrentStackTest, ConcurrentPushAndPop2) {
  ConcurrentStack<int> stack;
  int num = 10000;
  std::thread t1([&stack, num]() {
    for (int i = 0; i < num; ++i) {
      stack.Push(i);
    }
  });
  std::thread t2([&stack, num]() {
    for (int i = num; i < num * 2; ++i) {
      stack.Push(i);
    }
  });
  std::thread t3([&stack, num]() {
    for (int i = 0; i < num; ++i) {
      stack.WaitAndPop();
    }
  });
  t1.join();
  t2.join();
  t3.join();
  ASSERT_EQ(stack.Size(), num);
}

TEST(ConcurrentStackTest, ConcurrentPushAndPop3) {
  ConcurrentStack<int> stack;
  int num = 100;
  std::thread t1([&stack, num]() {
    for (int i = 0; i < num * 2; ++i) {
      stack.Push(i);
      std::this_thread::sleep_for(std::chrono::microseconds(10));
    }
  });
  std::thread t2([&stack, num]() {
    for (int i = 0; i < num * 2; ++i) {
      stack.Push(i);
      std::this_thread::sleep_for(std::chrono::microseconds(10));
    }
  });
  std::thread t3([&stack, num]() {
    for (int i = 0; i < num; ++i) {
      ASSERT_NE(nullptr, stack.WaitAndPop());
    }
  });
  std::thread t4([&stack, num]() {
    for (int i = 0; i < num; ++i) {
      ASSERT_NE(nullptr, stack.WaitAndPop());
    }
  });
  t1.join();
  t2.join();
  t3.join();
  t4.join();
  ASSERT_EQ(stack.Size(), num * 2);
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
