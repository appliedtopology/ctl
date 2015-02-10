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

#zoom zoom
set(CMAKE_BUILD_TYPE Release)

#Frame pointers are useful for profiling. 
#Complaints and pedantry accepted here.
add_definitions(" -fno-omit-frame-pointer -Wall -ansi -std=c++11")

#Fix issues on LINUX
if (NOT APPLE)
	set(DSO_FIXES rt)
endif (NOT APPLE)

#Handle dependencies
include( dependencies/CMakeLists.txt)

#Compile all the tools
add_subdirectory( tools)

#Compile all the tools
add_subdirectory( tests)


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
