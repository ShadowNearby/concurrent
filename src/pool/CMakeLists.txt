add_library(concurrent_thread_pool
        STATIC
        thread_pool.cpp)
set(ALL_OBJECT_FILES
        ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:concurrent_thread_pool>
        PARENT_SCOPE)