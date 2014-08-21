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

option(USE_MPI "build distributed codes" OFF) #OFF is default
set(CMAKE_BUILD_TYPE RELWITHDEBINFO)

set(CTL_INCLUDE_INSTALL_DIR ${CMAKE_SOURCE_DIR}/include)
set(CTL_DOC_INSTALL_DIR ${CMAKE_SOURCE_DIR}/doc)
set(CTL_BIN_INSTALL_DIR ${CMAKE_SOURCE_DIR}/bin)
set(CTL_EXAMPLES_INSTALL_DIR ${CMAKE_SOURCE_DIR}/examples)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMakeModules")
if (NOT APPLE)
	set(DSO_FIXES rt)
endif (NOT APPLE)

if(NOT USE_MPI)
  find_package(Boost COMPONENTS program_options serialization REQUIRED)
  link_directories(/usr/local/lib ${Boost_LIBRARY_DIR})
endif(NOT USE_MPI)
if( USE_MPI)
  set(MPI_CXX_COMPILER mpic++)
  find_package(MPI REQUIRED)
  find_package(Boost COMPONENTS program_options mpi serialization REQUIRED)
  include_directories(${MPI_INCLUDE_PATH})
  link_directories(${MPI_LIBRARY_DIRS})
endif(  USE_MPI)

set(Boost_USE_MULTITHREADED ON)
add_definitions(" -fno-omit-frame-pointer -Wall -ansi -std=c++11")

find_package(TBB REQUIRED) 
find_package(METIS REQUIRED) 
find_package(Doxygen)
if(DOXYGEN_FOUND)
configure_file(${CMAKE_SOURCE_DIR}/doc/Doxyfile.in ${CMAKE_BINARY_DIR}/Doxyfile @ONLY)
add_custom_target(doc
${DOXYGEN_EXECUTABLE} ${CMAKE_BINARY_DIR}/Doxyfile
WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
COMMENT "Generating API documentation with Doxygen" VERBATIM
)
endif(DOXYGEN_FOUND)


include_directories(${CMAKE_SOURCE_DIR} ${Boost_INCLUDE_DIR})
include_directories(${METIS_INCLUDE_DIR} ${TBB_INCLUDE_DIR})

link_directories(/usr/local/lib ${Boost_LIBRARY_DIR})
link_directories(${METIS_LIBRARY_DIR} ${TBB_LIBRARY_DIR})

add_subdirectory( ctl)

install( DIRECTORY ${CMAKE_SOURCE_DIR}/ctl 
	 DESTINATION include 
	 FILES_MATCHING PATTERN "*.h"
	 PATTERN "CMakeFiles" EXCLUDE)

install( DIRECTORY ${CMAKE_SOURCE_DIR}/ctl
	 DESTINATION share/ctl/examples
	 FILES_MATCHING PATTERN "*.cpp"
	 PATTERN "CMakeLists.txt"
	 PATTERN "CMakeFiles" EXCLUDE)
