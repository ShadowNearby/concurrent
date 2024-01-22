//
// Created by yanjingsheng on 2024/1/22.
//
#include "pool/thread_pool.h"
#include <gtest/gtest.h>
#include <random>
#include "common/duration_tracker.h"

namespace concurrent {

TEST(ThreadPoolTest, Basic) {
  int tasks = 1000000;
  std::vector<int> result(tasks, 0);
  {
    DurationTracker<std::chrono::milliseconds> tracker{"parallel exec"};
    ThreadPool pool(4);
    for (int i = 0; i < tasks; ++i) {
      pool.Commit([&result](int i) { result[i] = i * 2; }, i);
    }
    pool.WaitAll();
  }
  for (int i = 0; i < tasks; ++i) {
    EXPECT_EQ(result[i], i * 2);
  }
}

template <typename RandomIterator>
void merge(RandomIterator first, RandomIterator middle, RandomIterator last) {
  std::vector<typename RandomIterator::value_type> tempBuffer(std::distance(first, last));
  auto left = first;
  auto right = middle;
  auto temp = tempBuffer.begin();

  while (left < middle && right < last) {
    *temp++ = (*left < *right) ? std::move(*left++) : std::move(*right++);
  }

  temp = std::move(left, middle, temp);
  temp = std::move(right, last, temp);

  std::move(tempBuffer.begin(), tempBuffer.end(), first);
}

template <typename RandomIterator>
void parallel_merge_sort(RandomIterator first, RandomIterator last, int threshold, concurrent::ThreadPool &pool) {
  auto size = std::distance(first, last);
  if (size <= threshold) {
    std::sort(first, last);
  } else {
    auto middle = first + size / 2;

    // 并行执行归并排序
    auto future = pool.Commit(parallel_merge_sort<RandomIterator>, first, middle, threshold, std::ref(pool));
    parallel_merge_sort(middle, last, threshold, pool);

    future.get();

    merge(first, middle, last);
  }
}

template <typename RandomIterator>
void parallel_merge_sort(RandomIterator first, RandomIterator last, int threshold) {
  concurrent::ThreadPool pool(std::thread::hardware_concurrency());
  parallel_merge_sort(first, last, threshold, pool);
}

TEST(ThreadPoolTest, MergeSort) {
  ThreadPool pool;
  int size = 10000000;
  std::vector<int> data(size);
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> distribution(1, size * 10);

  // 生成随机整数序列
  for (int i = 0; i < size; ++i) {
    data[i] = distribution(gen);
  }
  parallel_merge_sort(data.begin(), data.end(), size / 8, pool);
  EXPECT_EQ(true, std::is_sorted(data.begin(), data.end()));
}

}  // namespace concurrent
