//
// Created by yanjingsheng on 2024/1/22.
//

#pragma once
#include <atomic>
#include <chrono>
#include <concepts>
#include <type_traits>
#include "common/logger.h"

namespace concurrent {
template <typename T>
concept TimeUnitConcept = std::same_as<T, std::chrono::seconds> || std::same_as<T, std::chrono::milliseconds> ||
                          std::same_as<T, std::chrono::microseconds> || std::same_as<T, std::chrono::nanoseconds> ||
                          std::same_as<T, std::chrono::seconds> || std::same_as<T, std::chrono::minutes> ||
                          std::same_as<T, std::chrono::hours> || std::same_as<T, std::chrono::days> ||
                          std::same_as<T, std::chrono::weeks> || std::same_as<T, std::chrono::years>;

template <TimeUnitConcept T>
class DurationTracker {
 public:
  explicit DurationTracker(std::string_view label);
  DurationTracker();
  DurationTracker(const DurationTracker &) = delete;
  auto operator=(const DurationTracker &) -> DurationTracker & = delete;

  ~DurationTracker() {
    if (end_) {
      return;
    }
    Print();
  };
  void End();
  uint64_t EndAndGetDuration();

 private:
  void Print();

  std::string_view label_;
  std::atomic<bool> end_;
  std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds> begin_time_;
};

template <TimeUnitConcept T>
DurationTracker<T>::DurationTracker(std::string_view label)
    : label_(label), end_(false), begin_time_(std::chrono::steady_clock::now()) {}

template <TimeUnitConcept T>
DurationTracker<T>::DurationTracker() : end_(false), begin_time_(std::chrono::steady_clock::now()) {}

template <TimeUnitConcept T>
void DurationTracker<T>::Print() {
  auto now = std::chrono::steady_clock::now();
  auto dur = std::chrono::duration_cast<T>(now - begin_time_).count();
  auto unit = std::string_view();
  if (std::is_same<T, std::chrono::seconds>::value) {
    unit = "s";
  } else if (std::is_same<T, std::chrono::milliseconds>::value) {
    unit = "ms";
  } else if (std::is_same<T, std::chrono::microseconds>::value) {
    unit = "us";
  } else if (std::is_same<T, std::chrono::nanoseconds>::value) {
    unit = "ns";
  } else if (std::is_same<T, std::chrono::minutes>::value) {
    unit = "min";
  } else if (std::is_same<T, std::chrono::hours>::value) {
    unit = "h";
  } else {
    LOG_FORMAT_ERROR("{}", "error time unit");
  }
  LOG_FORMAT_INFO("{} duration: {}{}", label_, dur, unit);
}

template <TimeUnitConcept T>
void DurationTracker<T>::End() {
  end_.store(true);
  Print();
}
template <TimeUnitConcept T>
uint64_t DurationTracker<T>::EndAndGetDuration() {
  end_.store(true);
  auto now = std::chrono::steady_clock::now();
  auto dur = std::chrono::duration_cast<T>(now - begin_time_).count();
  return dur;
}
}  // namespace concurrent
