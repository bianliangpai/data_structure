CMAKE_MINIMUM_REQUIRED(VERSION 3.0.0)
project(test_list)

include_directories(include)

add_executable(${PROJECT_NAME} test/linear/${PROJECT_NAME}.cc)
