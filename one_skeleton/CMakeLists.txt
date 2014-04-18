# Create library for module

# Make sure the compiler can find include files
include_directories( ${CMAKE_SOURCE_DIR})

# Test executable
add_executable (test_one_skeleton test_one_skeleton.cpp)
