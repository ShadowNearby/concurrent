add_library(concurrent_queue
        OBJECT
        concurrent_queue.cpp)
set(ALL_OBJECT_FILES
        ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:concurrent_queue>
        PARENT_SCOPE)