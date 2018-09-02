enable_testing()
find_package(GTest REQUIRED)

file(GLOB TEST_FILES "*.cpp")
set(TEST_NAME "unitTest")

add_executable(${TEST_NAME} ${TEST_FILES})
target_link_libraries(${TEST_NAME} ${PROJECT_NAME} ${GTEST_BOTH_LIBRARIES})
add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

