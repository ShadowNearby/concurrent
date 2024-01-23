add_subdirectory(queue)
add_subdirectory(pool)
add_subdirectory(common)
add_subdirectory(stack)

add_library(concurrent STATIC ${ALL_OBJECT_FILES})

set(CONCURRENT_LIBS
        concurrent_queue
        concurrent_thread_pool
        concurrent_common
)

find_package(Threads REQUIRED)

set(CONCURRENT_THIRDPARTY_LIBS
        fmt
        Threads::Threads
)

target_link_libraries(
        concurrent
        ${CONCURRENT_LIBS}
        ${CONCURRENT_THIRDPARTY_LIBS})

target_include_directories(
        concurrent PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
