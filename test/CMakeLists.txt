
set(PROJECT_TEST_SRCS
  runner.cpp
  test_cases.cpp
)

enable_testing()

add_executable(${PROJECT_NAME}_test
  ${PROJECT_TEST_SRCS}
)

target_link_libraries(${PROJECT_NAME}_test
  gtest
  gmock
  gtest_main
)

add_test(${PROJECT_NAME}_test ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}_test)
