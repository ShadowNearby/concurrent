//
// Created by yanjingsheng on 2024/1/22.
//
#include "common/duration_tracker.h"
#include "common/logger.h"

namespace concurrent {
template class DurationTracker<std::chrono::nanoseconds>;
template class DurationTracker<std::chrono::microseconds>;
template class DurationTracker<std::chrono::milliseconds>;
template class DurationTracker<std::chrono::seconds>;
template class DurationTracker<std::chrono::hours>;
template class DurationTracker<std::chrono::days>;
template class DurationTracker<std::chrono::weeks>;
template class DurationTracker<std::chrono::years>;
template class DurationTracker<std::chrono::months>;
}  // namespace concurrent
