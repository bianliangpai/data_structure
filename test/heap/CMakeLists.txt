CMAKE_MINIMUM_REQUIRED(VERSION 3.0.0)
project(test_binary_heap)

include_directories(include)

add_executable(${PROJECT_NAME} test/heap/${PROJECT_NAME}.cc)
