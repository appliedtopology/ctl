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
option(USE_CLANG "build application with clang" OFF) # OFF is the default
option(USE_MPI "build distributed codes" OFF) #OFF is default

if( USE_CLANG)
   SET (CMAKE_C_COMPILER             "/usr/bin/clang")
   SET (CMAKE_C_FLAGS                "-Wall -std=c99")
   SET (CMAKE_C_FLAGS_DEBUG          "-g")
   SET (CMAKE_C_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
   SET (CMAKE_C_FLAGS_RELEASE        "-O4 -DNDEBUG")
   SET (CMAKE_C_FLAGS_RELWITHDEBINFO "-O2 -g")

   SET (CMAKE_CXX_COMPILER             "/usr/bin/clang++")
   SET (CMAKE_CXX_FLAGS                "-Wall")
   SET (CMAKE_CXX_FLAGS_DEBUG          "-g")
   SET (CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
   SET (CMAKE_CXX_FLAGS_RELEASE        "-O3 -DNDEBUG")
   SET (CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g -fno-omit-frame-pointer")
   SET (CMAKE_AR      "/usr/bin/llvm-ar")
   SET (CMAKE_LINKER  "/usr/bin/llvm-link")
   SET (CMAKE_NM      "/usr/bin/llvm-nm")
   SET (CMAKE_OBJDUMP "/usr/bin/llvm-objdump")
   SET (CMAKE_RANLIB  "/usr/bin/llvm-ranlib")
endif( USE_CLANG)

set(CMAKE_BUILD_TYPE RELWITHDEBINFO)

set(CTL_INCLUDE_INSTALL_DIR ${CMAKE_SOURCE_DIR}/include)
set(CTL_DOC_INSTALL_DIR ${CMAKE_SOURCE_DIR}/doc)
set(CTL_BIN_INSTALL_DIR ${CMAKE_SOURCE_DIR}/bin)
set(CTL_EXAMPLES_INSTALL_DIR ${CMAKE_SOURCE_DIR}/examples)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMakeModules")

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

set(Boost_USE_MULTITHREADED OFF)
add_definitions(" -fno-omit-frame-pointer -Wall -ansi -std=c++11")

find_package(TBB REQUIRED) 
find_package(METIS REQUIRED) 

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
