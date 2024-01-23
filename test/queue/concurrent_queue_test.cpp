//
// Created by yanjingsheng on 2024/1/22.
//
#include "queue/concurrent_queue.h"

#include <fmt/core.h>
#include <gtest/gtest.h>

#include <thread>

namespace concurrent {
TEST(ConcurrentQueueTest, PushAndPop) {
  ConcurrentQueue<int> queue;
  int num = 10;
  for (int i = 0; i < num; ++i) {
    queue.Push(i);
  }
  for (int i = 0; i < num; ++i) {
    ASSERT_EQ(*queue.WaitAndPop(), i);
  }
  ASSERT_TRUE(queue.Empty());
}

TEST(ConcurrentQueueTest, ConcurrentPushAndPop) {
  ConcurrentQueue<int> queue;
  int num = 100;
  std::thread t1([&queue, num]() {
    for (int i = 0; i < num; ++i) {
      queue.Push(i);
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
  });
  std::thread t2([&queue, num]() {
    for (int i = 0; i < num; ++i) {
      ASSERT_EQ(*queue.WaitAndPop(), i);
    }
    ASSERT_TRUE(queue.Empty());
  });
  t1.join();
  t2.join();
}

TEST(ConcurrentQueueTest, ConcurrentPush) {
  ConcurrentQueue<int> queue;
  int num = 10000;
  std::thread t1([&queue, num]() {
    for (int i = 0; i < num; ++i) {
      queue.Push(i);
    }
  });
  std::thread t2([&queue, num]() {
    for (int i = num; i < num * 2; ++i) {
      queue.Push(i);
    }
  });
  t1.join();
  t2.join();
  ASSERT_EQ(queue.Size(), num * 2);
}

TEST(ConcurrentQueueTest, ConcurrentPushAndPop2) {
  ConcurrentQueue<int> queue;
  int num = 10000;
  std::thread t1([&queue, num]() {
    for (int i = 0; i < num; ++i) {
      queue.Push(i);
    }
  });
  std::thread t2([&queue, num]() {
    for (int i = num; i < num * 2; ++i) {
      queue.Push(i);
    }
  });
  std::thread t3([&queue, num]() {
    for (int i = 0; i < num; ++i) {
      queue.WaitAndPop();
    }
  });
  t1.join();
  t2.join();
  t3.join();
  ASSERT_EQ(queue.Size(), num);
}

TEST(ConcurrentQueueTest, ConcurrentPushAndPop3) {
  ConcurrentQueue<int> queue;
  int num = 100;
  std::thread t1([&queue, num]() {
    for (int i = 0; i < num * 2; ++i) {
      queue.Push(i);
      std::this_thread::sleep_for(std::chrono::microseconds(10));
    }
  });
  std::thread t2([&queue, num]() {
    for (int i = 0; i < num * 2; ++i) {
      queue.Push(i);
      std::this_thread::sleep_for(std::chrono::microseconds(10));
    }
  });
  std::thread t3([&queue, num]() {
    for (int i = 0; i < num; ++i) {
      ASSERT_NE(nullptr, queue.WaitAndPop());
    }
  });
  std::thread t4([&queue, num]() {
    for (int i = 0; i < num; ++i) {
      ASSERT_NE(nullptr, queue.WaitAndPop());
    }
  });
  t1.join();
  t2.join();
  t3.join();
  t4.join();
  ASSERT_EQ(queue.Size(), num * 2);
}

TEST(ConcurrentQueueTest, TryPop) {
  ConcurrentQueue<int> queue;
  int num = 10;
  for (int i = 0; i < num; ++i) {
    int value = i;
    ASSERT_EQ(queue.TryPop(), nullptr);
    queue.Push(value);
    ASSERT_EQ(*queue.TryPop(), value);
  }
}
}  // namespace concurrent