cmake_minimum_required(VERSION 3.10)

# Set the project name
project(chip8)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Add the executable
add_executable(chip8 src/main.cpp src/cpu.cpp src/memory.cpp src/utils.cpp src/renderer.cpp)

find_package(SFML 2.5 COMPONENTS graphics audio window system REQUIRED)
target_link_libraries(chip8 sfml-graphics sfml-audio sfml-window sfml-system)

# Enable testing
enable_testing()

# Add Google Test directory
add_subdirectory(googletest)

# Add test executable
add_executable(chip8_tests tests/test.cpp src/cpu.cpp src/memory.cpp src/utils.cpp src/renderer.cpp)

# Link test executable against gtest libraries AND the necessary SFML libraries
target_link_libraries(chip8_tests gtest gtest_main sfml-graphics sfml-audio sfml-window sfml-system)

# Add the tests to CTest
add_test(NAME chip8_tests COMMAND chip8_tests)

