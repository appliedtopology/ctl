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

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++1y" COMPILER_SUPPORTS_CXX1y)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
	add_definitions("-std=c++11")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has C++11 support.")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

#zoom zoom
set(CMAKE_BUILD_TYPE Release)

#Frame pointers are useful for profiling. 
#Complaints and pedantry accepted here.
add_definitions("-fno-omit-frame-pointer -pedantic -Wall -ansi -std=c++11")

#Fix issues on LINUX
if (NOT APPLE)
	set(DSO_FIX rt)
	set(PTHREAD_LIBRARY pthread)
endif (NOT APPLE)

#Handle dependencies
set(CTL_DEPS ${CMAKE_SOURCE_DIR}/dependencies)
include( dependencies/CMakeLists.txt)

#Compile all the tools
add_subdirectory( tools)

#Compile all the tools
add_subdirectory( tests)

enable_testing()
add_test(gtest tests/ctl_test)

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
