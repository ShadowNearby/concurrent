add_subdirectory(queue)
add_subdirectory(pool)
add_subdirectory(common)
add_library(concurrent STATIC ${ALL_OBJECT_FILES})

find_package(Threads REQUIRED)

set(CONCURRENT_THIRDPARTY_LIBS
        fmt
        Threads::Threads
)