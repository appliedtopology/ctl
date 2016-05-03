project(ctl CXX)
#stop the whining
cmake_minimum_required(VERSION 2.6.2)
if("${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}" VERSION_GREATER 2.6)
  if("${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION}" VERSION_GREATER 2.8.3)
    cmake_policy(VERSION 2.8.4)
  else()
    cmake_policy(VERSION 2.6)
  endif()
endif()


# CTL must be built "out-of-source", so we start by ensuring that the
# source and build directories are different.
if("${PROJECT_SOURCE_DIR}" STREQUAL "${PROJECT_BINARY_DIR}")
  message(FATAL_ERROR "In-source build attempted; please clean the CMake cache and then switch to an out-of-source build, e.g., rm CMakeCache.txt && rm -Rf CMakeFiles/ && mkdir build/ && cd build/ && cmake ..")
endif()

set(CMAKE_BUILD_TYPE Release)

#Frame pointers are useful for profiling. 
#Complaints and pedantry accepted here.
add_definitions("-fno-omit-frame-pointer -pedantic -Wall -ansi -std=c++14")

#Handle dependencies
set(CTL_DEPS ${CMAKE_SOURCE_DIR}/dependencies)
include( dependencies/CMakeLists.txt)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CTL_DEPS})
include_directories(${CTL_DEPS}/pybind11/include/)
#Compile all the tools
add_subdirectory( tools)

#Compile all the tools
enable_testing()
add_subdirectory( tests)
add_subdirectory( python)

#All of this is for make install
set(CTL_INCLUDE_INSTALL_DIR ${CMAKE_SOURCE_DIR}/include)
set(CTL_DOC_INSTALL_DIR ${CMAKE_SOURCE_DIR}/doc)
set(CTL_BIN_INSTALL_DIR ${CMAKE_SOURCE_DIR}/bin)
set(CTL_EXAMPLES_INSTALL_DIR ${CMAKE_SOURCE_DIR}/examples)

install( DIRECTORY ${CMAKE_SOURCE_DIR}/ctl 
	 DESTINATION include 
	 FILES_MATCHING PATTERN "*.h"
	 PATTERN "CMakeFiles" EXCLUDE)

install( DIRECTORY ${CMAKE_SOURCE_DIR}/tools
	 DESTINATION share/ctl/tools
	 FILES_MATCHING PATTERN "*.cpp"
	 PATTERN "CMakeLists.txt"
	 PATTERN "CMakeFiles" EXCLUDE)
