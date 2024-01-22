add_library(concurrent_common
        OBJECT
        duration_tracker.cpp)
set(ALL_OBJECT_FILES
        ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:concurrent_common>
        PARENT_SCOPE)