# Created by the script cgal_create_cmake_script
# This is the CMake script for compiling a CGAL application.


project( ctlib CXX)
set( CMAKE_VERBOSE_MAKEFILE  false)
#set (CMAKE_INSTALL_PREFIX /usr/local)
#for installing read:
#http://www.cmake.org/cmake/help/cmake_tutorial.html
set(Boost_USE_MULTITHREADED OFF)
find_package(Boost COMPONENTS program_options REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
link_directories(/usr/local/lib ${Boost_LIBRARY_DIR})

add_definitions("-std=c++11")
set(CMAKE_BUILD_TYPE Release) 
cmake_minimum_required(VERSION 2.6.2)
if("${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}" VERSION_GREATER 2.6)
  if("${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION}" VERSION_GREATER 2.8.3)
    cmake_policy(VERSION 2.8.4)
  else()
    cmake_policy(VERSION 2.6)
  endif()
endif()

add_subdirectory( term)
add_subdirectory( abstract_simplex)
add_subdirectory( chain)
add_subdirectory( chain_complex)
add_subdirectory( finite_field)
add_subdirectory( filtration)
add_subdirectory( tools)
