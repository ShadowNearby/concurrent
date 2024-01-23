add_library(concurrent_stack
        OBJECT
        concurrent_stack.cpp)
set(ALL_OBJECT_FILES
        ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:concurrent_stack>
        PARENT_SCOPE)