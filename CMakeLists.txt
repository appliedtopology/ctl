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

#This matters
set(CMAKE_BUILD_TYPE Release)

#Frame pointers are useful for profiling.
add_definitions(" -fno-omit-frame-pointer -Wall -ansi -std=c++11")

#For make install
set(CTL_INCLUDE_INSTALL_DIR ${CMAKE_SOURCE_DIR}/include)
set(CTL_DOC_INSTALL_DIR ${CMAKE_SOURCE_DIR}/doc)
set(CTL_BIN_INSTALL_DIR ${CMAKE_SOURCE_DIR}/bin)
set(CTL_EXAMPLES_INSTALL_DIR ${CMAKE_SOURCE_DIR}/examples)

#Fix issues on LINUX
if (NOT APPLE)
	set(DSO_FIXES rt)
endif (NOT APPLE)

#Handle dependencies
add_subdirectory( dependencies)

include_directories(${CMAKE_SOURCE_DIR} ${Boost_INCLUDE_DIR})
include_directories(/usr/local/include ${MPI_INCLUDE_DIR})
include_directories(${ANN_INCLUDE_DIRS} ${GTEST_INCLUDE_DIR})
include_directories(${METIS_INCLUDE_DIR} ${TBB_INCLUDE_DIR})

link_directories(/usr/local/lib ${ANN_LIBRARY_DIR} ${Boost_LIBRARY_DIR})
link_directories( ${MPI_LIBRARY_DIR} ${GTEST_LIBRARY_DIR})
link_directories(/usr/local/lib ${Boost_LIBRARY_DIR})
link_directories(${METIS_LIBRARY_DIR} ${TBB_LIBRARY_DIR})

add_subdirectory( ctl)
add_subdirectory( tools)

install( DIRECTORY ${CMAKE_SOURCE_DIR}/ctl 
	 DESTINATION include 
	 FILES_MATCHING PATTERN "*.h"
	 PATTERN "CMakeFiles" EXCLUDE)

install( DIRECTORY ${CMAKE_SOURCE_DIR}/tools
	 DESTINATION share/ctl/tools
	 FILES_MATCHING PATTERN "*.cpp"
	 PATTERN "CMakeLists.txt"
	 PATTERN "CMakeFiles" EXCLUDE)
