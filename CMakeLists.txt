cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
	googletest
	URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
)
FetchContent_MakeAvailable(googletest)

project(minishellTests)
target_include_directories(minishellTests PRIVATE ./minishell/includes/)
add_subdirectory(src)
add_executable(minishellTests ./tests/tokenTests.cpp)
target_link_libraries(minishellTests gtest_main)

enable_testing()
add_subdirectory(tests)