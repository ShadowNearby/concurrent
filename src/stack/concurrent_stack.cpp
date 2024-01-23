//
// Created by yanjingsheng on 2024/1/23.
//

#include "stack/concurrent_stack.h"

#include <string_view>
#include "common/type.h"

namespace concurrent {
template class ConcurrentStack<int>;
template class ConcurrentStack<std::string_view>;
template class ConcurrentStack<Integer>;
}  // namespace concurrent
