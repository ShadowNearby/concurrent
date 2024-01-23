//
// Created by yanjingsheng on 2024/1/22.
//
#include <queue/concurrent_queue.h>
#include "common/type.h"

namespace concurrent {
template class ConcurrentQueue<int>;
template class ConcurrentQueue<std::string_view>;
template class ConcurrentQueue<Integer>;
}  // namespace concurrent