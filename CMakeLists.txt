project(ctl CXX)
set( CMAKE_VERBOSE_MAKEFILE  false)
cmake_minimum_required(VERSION 2.6.2)
if("${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}" VERSION_GREATER 2.6)
  if("${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION}" VERSION_GREATER 2.8.3)
    cmake_policy(VERSION 2.8.4)
  else()
    cmake_policy(VERSION 2.6)
  endif()
endif()

set(CMAKE_BUILD_TYPE Release)
add_definitions(" -fno-omit-frame-pointer -Wall -ansi -std=c++11")

set(CTL_INCLUDE_INSTALL_DIR ${CMAKE_SOURCE_DIR}/include)
set(CTL_DOC_INSTALL_DIR ${CMAKE_SOURCE_DIR}/doc)
set(CTL_BIN_INSTALL_DIR ${CMAKE_SOURCE_DIR}/bin)
set(CTL_EXAMPLES_INSTALL_DIR ${CMAKE_SOURCE_DIR}/examples)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMakeModules")
set(CMAKE_PREFIX_PATH /usr/lib64/openmpi)

if (NOT APPLE)
	set(DSO_FIXES rt)
endif (NOT APPLE)

find_package(Boost COMPONENTS program_options serialization mpi REQUIRED)
find_package(MPI REQUIRED)
find_package(ANN REQUIRED)
find_package(TBB REQUIRED) 
find_package(METIS REQUIRED) 
find_package(Doxygen)

set(GTEST_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/deps/gtest/include)
message( ${GTEST_INCLUDE_DIR})
set(GTEST_LIBRARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/deps/gtest)
set(GTEST_LIBRARIES gtest gtest_main)

if(DOXYGEN_FOUND)
configure_file(${CMAKE_SOURCE_DIR}/doc/Doxyfile.in 
	       ${CMAKE_BINARY_DIR}/Doxyfile @ONLY)
add_custom_target(doc ${DOXYGEN_EXECUTABLE} ${CMAKE_BINARY_DIR}/Doxyfile
WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
COMMENT "Generating API documentation with Doxygen" VERBATIM
)
endif(DOXYGEN_FOUND)

add_custom_target(deps COMMAND cmake . COMMAND make 
		  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/deps)

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
#add_subdirectory( distributed)

install( DIRECTORY ${CMAKE_SOURCE_DIR}/ctl 
	 DESTINATION include 
	 FILES_MATCHING PATTERN "*.h"
	 PATTERN "CMakeFiles" EXCLUDE)

install( DIRECTORY ${CMAKE_SOURCE_DIR}/tools
	 DESTINATION share/ctl/tools
	 FILES_MATCHING PATTERN "*.cpp"
	 PATTERN "CMakeLists.txt"
	 PATTERN "CMakeFiles" EXCLUDE)
