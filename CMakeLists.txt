cmake_minimum_required(VERSION 3.10)

# Set the project name
project(chip8)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Add the executable
add_executable(chip8 src/main.cpp src/cpu.cpp src/memory.cpp src/utils.cpp)

# Enable testing
enable_testing()

# Add Google Test directory
add_subdirectory(googletest)

# Add test executable
add_executable(chip8_tests tests/test.cpp src/cpu.cpp src/memory.cpp src/utils.cpp)

# Link test executable against gtest libraries
target_link_libraries(chip8_tests gtest gtest_main)

# Add the tests to CTest
add_test(NAME chip8_tests COMMAND chip8_tests)

