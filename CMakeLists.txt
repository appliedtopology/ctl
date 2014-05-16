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

option(USE_GCC "build application with gcc" ON) # ON is the default
if( USE_GCC)
   SET (CMAKE_C_COMPILER             "/usr/local/bin/gcc")
   SET (CMAKE_C_FLAGS                "-Wall -std=c99")
   SET (CMAKE_C_FLAGS_DEBUG          "-g")
   SET (CMAKE_C_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
   SET (CMAKE_C_FLAGS_RELEASE        "-O3 -DNDEBUG")
   SET (CMAKE_C_FLAGS_RELWITHDEBINFO "-O2 -g")

   SET (CMAKE_CXX_COMPILER             "/usr/bin/clang++")
   SET (CMAKE_CXX_FLAGS                "-Wall")
   SET (CMAKE_CXX_FLAGS_DEBUG          "-g")
   SET (CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
   SET (CMAKE_CXX_FLAGS_RELEASE        "-O3 -DNDEBUG")
   SET (CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g -fno-omit-frame-pointer")
endif( USE_GCC)
option(USE_CLANG "build application with clang" OFF) # OFF is the default
if( USE_CLANG)
   SET (CMAKE_C_COMPILER             "/usr/bin/clang")
   SET (CMAKE_C_FLAGS                "-Wall -std=c99")
   SET (CMAKE_C_FLAGS_DEBUG          "-g")
   SET (CMAKE_C_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
   SET (CMAKE_C_FLAGS_RELEASE        "-O3 -DNDEBUG")
   SET (CMAKE_C_FLAGS_RELWITHDEBINFO "-O2 -g")

   SET (CMAKE_CXX_COMPILER             "/usr/bin/clang++")
   SET (CMAKE_CXX_FLAGS                "-Wall")
   SET (CMAKE_CXX_FLAGS_DEBUG          "-g")
   SET (CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
   SET (CMAKE_CXX_FLAGS_RELEASE        "-O3 -DNDEBUG")
   SET (CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g -fno-omit-frame-pointer")
endif( USE_CLANG)
#set (CMAKE_INSTALL_PREFIX /usr/local)
#for installing read:
#http://www.cmake.org/cmake/help/cmake_tutorial.html
set(CTL_INCLUDE_INSTALL_DIR ${CMAKE_SOURCE_DIR}/include)
set(CTL_DOC_INSTALL_DIR ${CMAKE_SOURCE_DIR}/doc)
set(CTL_BIN_INSTALL_DIR ${CMAKE_SOURCE_DIR}/bin)
set(CTL_EXAMPLES_INSTALL_DIR ${CMAKE_SOURCE_DIR}/examples)
set(Boost_REALPATH ON)
set(Boost_ADDITIONAL_VERSIONS 1.55.0_1)
set(Boost_DETAILED_FAILURE_MSG OFF)
set(Boost_DEBUG OFF)
set(Boost_USE_MULTITHREADED ON)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMakeModules")
set(MPI_CXX_COMPILER mpic++)
find_package(MPI REQUIRED)
#set(Boost_USE_MULTITHREADED OFF)
find_package(Boost COMPONENTS program_options mpi serialization REQUIRED)
find_package(TBB REQUIRED) 
find_package(METIS REQUIRED) 
include_directories(${CMAKE_SOURCE_DIR})
include_directories( ${MPI_INCLUDE_PATH} ${METIS_INCLUDE_DIR} ${Boost_INCLUDE_DIR} ${TBB_INCLUDE_DIR})
link_directories(/usr/local/lib ${Boost_LIBRARY_DIR} ${METIS_LIBRARY_DIR} ${TBB_LIBRARY_DIR} ${MPI_LIBRARY_DIRS})
add_definitions(" -fno-omit-frame-pointer -ftemplate-backtrace-limit=0 -Wall -ansi -std=c++11")
set(CMAKE_BUILD_TYPE RELEASE)
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
