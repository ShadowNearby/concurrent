cmake_minimum_required(VERSION 3.22)
project(concurrent)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

if (NOT DEFINED CONCURRENT_SANITIZER)
    set(CONCURRENT_SANITIZER address)
endif ()

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -Werror")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -ggdb -fsanitize=${CONCURRENT_SANITIZER} -fno-omit-frame-pointer -fno-optimize-sibling-calls")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

message(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
message(STATUS "CMAKE_CXX_FLAGS_DEBUG: ${CMAKE_CXX_FLAGS_DEBUG}")
message(STATUS "CMAKE_EXE_LINKER_FLAGS: ${CMAKE_EXE_LINKER_FLAGS}")
message(STATUS "CMAKE_SHARED_LINKER_FLAGS: ${CMAKE_SHARED_LINKER_FLAGS}")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CONCURRENT_SRC_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(CONCURRENT_TEST_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/test/include)
set(CONCURRENT_THIRD_PARTY_INCLUDE_DIR
        ${PROJECT_SOURCE_DIR}/third_party
        ${PROJECT_SOURCE_DIR}/third_party/fmt/include
        ${PROJECT_SOURCE_DIR}/third_party/googletest/googlemock/include
        ${PROJECT_SOURCE_DIR}/third_party/googletest/googletest/include
)

include_directories(${CONCURRENT_SRC_INCLUDE_DIR} ${CONCURRENT_TEST_INCLUDE_DIR} ${CONCURRENT_THIRD_PARTY_INCLUDE_DIR})
include_directories(BEFORE src)

add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(third_party)

add_executable(main main.cpp)

target_link_libraries(main concurrent)
