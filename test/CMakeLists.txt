include(GoogleTest)
file(GLOB_RECURSE CONCURRENT_TEST_SOURCES "${PROJECT_SOURCE_DIR}/test/*/*test.cpp")
add_custom_target(build-tests COMMAND ${CMAKE_CTEST_COMMAND} --show-only)
add_custom_target(check-tests COMMAND ${CMAKE_CTEST_COMMAND} --verbose)

foreach (concurrent_test_source ${CONCURRENT_TEST_SOURCES})
    # Create a human readable name.
    get_filename_component(concurrent_test_filename ${concurrent_test_source} NAME)
    string(REPLACE ".cpp" "" concurrent_test_name ${concurrent_test_filename})

    # Add the test target separately and as part of "make check-tests".
    add_executable(${concurrent_test_name} EXCLUDE_FROM_ALL ${concurrent_test_source})
    add_dependencies(build-tests ${concurrent_test_name})
    add_dependencies(check-tests ${concurrent_test_name})

    gtest_discover_tests(${concurrent_test_name}
            EXTRA_ARGS
            --gtest_color=auto
            --gtest_output=xml:${CMAKE_BINARY_DIR}/test/${concurrent_test_name}.xml
            --gtest_catch_exceptions=0
            DISCOVERY_TIMEOUT 120
            PROPERTIES
            TIMEOUT 120
    )

    target_link_libraries(${concurrent_test_name} concurrent gtest gmock_main fmt)

    # Set test target properties and dependencies.
    set_target_properties(${concurrent_test_name}
            PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/test"
            COMMAND ${concurrent_test_name}
    )
endforeach ()
