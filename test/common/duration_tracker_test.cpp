//
// Created by yanjingsheng on 2024/1/22.
//
#include "common/duration_tracker.h"
#include <fmt/core.h>
#include <gtest/gtest.h>
#include <thread>

namespace concurrent {
TEST(CommonTest, DurationTrackerTestBasic) {
  std::vector<uint32_t> durs = {100, 200, 300, 400, 500};
  for (const auto &expect_dur : durs) {
    DurationTracker<std::chrono::milliseconds> tracker{};
    std::this_thread::sleep_for(std::chrono::milliseconds(expect_dur));
    auto actual_dur = tracker.EndAndGetDuration();
    EXPECT_GE(actual_dur, expect_dur - 10);
    EXPECT_LE(actual_dur, expect_dur + 10);
  }
}
}  // namespace concurrent