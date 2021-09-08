CMAKE_MINIMUM_REQUIRED(VERSION 3.0.0)
project(test_node)

include_directories(include)

add_executable(${PROJECT_NAME} test/base/${PROJECT_NAME}.cc)
