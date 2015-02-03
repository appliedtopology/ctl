set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/dependencies/CMakeModules")
set(CMAKE_PREFIX_PATH /usr/lib64/openmpi)

include(ExternalProject)
include( ${CMAKE_SOURCE_DIR}/build_dependencies.txt RESULT_VARIABLE bdvars)
#if( bdvars NOTFOUND)
##TODO: Check if not found
#endif()

if( NOT Boost_FOUND)
	find_package(Boost COMPONENTS program_options serialization mpi)
	if( NOT Boost_FOUND)
	 message( "--- Boost not found. Install locally by typing 'make boost'")
	endif() 
else(Boost_FOUND)
	message("-- Using Boost specified by user")
endif()

if( NOT MPI_FOUND)
	find_package(MPI REQUIRED)
elseif( MPI_FOUND)
	message( "-- Using MPI specified by user")
endif()

if( NOT TBB_FOUND)
	find_package(TBB) 
elseif( TBB_FOUND)
	message( "-- Using TBB specified by user")
endif()

if( NOT ANN_FOUND)
	find_package(ANN)
	if( NOT ANN_FOUND)
	 message( "--- ANN not found. Install locally by typing 'make ANN'")
	endif()
elseif( ANN_FOUND)
	message( "-- Using ANN specified by user")
endif()

if( NOT METIS_FOUND)
	find_package(METIS)
	if( NOT METIS_FOUND)
	 message( "--- METIS not found. Install locally by typing 'make metis'")
	endif()
elseif( METIS_FOUND)
	message( "-- Using METIS specified by user")
endif()

if( NOT DOXYGEN_FOUND)
	find_package(Doxygen)
elseif( DOXYGEN_FOUND)
	message( "-- Using Doxygen specified by user")
endif() 

find_package(GTest REQUIRED)

if( GTest_FOUND)
   set(GTEST_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/gtest/include)
   set(GTEST_LIBRARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/gtest)
   set(GTEST_LIBRARIES gtest gtest_main)
endif()

if(DOXYGEN_FOUND)
configure_file(${CMAKE_SOURCE_DIR}/doc/Doxyfile.in 
	       ${CMAKE_BINARY_DIR}/Doxyfile @ONLY)
add_custom_target(doc ${DOXYGEN_EXECUTABLE} ${CMAKE_BINARY_DIR}/Doxyfile
WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
COMMENT "Generating API documentation with Doxygen" VERBATIM
)
endif(DOXYGEN_FOUND)

if( OSX)
 set(ANN_VAR macos-g++)
elseif( NOT OSX)
 set(ANN_VAR linux-g++)
endif( OSX)

ExternalProject_add(ann
PREFIX ann
URL http://www.cs.umd.edu/~mount/ANN/Files/${ANN_Version}/ann_${ANN_Version}.zip
URL_MD5 31267ffbe4e6d04768b3ec21763e9343
CONFIGURE_COMMAND ""
BUILD_COMMAND make ${ANN_VAR}
BUILD_IN_SOURCE 1
INSTALL_COMMAND ""
)

ExternalProject_Add(boost
PREFIX ${CMAKE_CURRENT_BINARY_DIR}/boost
URL http://downloads.sourceforge.net/project/boost/boost/${Boost_Version}/boost_${Boost_Version_Underscore}.zip
URL_MD5 854dcbbff31b896c85c38247060b7713
UPDATE_COMMAND "bootstrap.bat"
CONFIGURE_COMMAND ""
BUILD_COMMAND bjam --with-program_options address-model=64 toolset=msvc-11.0 link=static stage
BUILD_IN_SOURCE 1
INSTALL_COMMAND ""
)


ExternalProject_Add(gtest
DEPENDS gtest
PREFIX gtest 
URL http://googletest.googlecode.com/files/gtest-1.7.0.zip
INSTALL_COMMAND cp -r -p libgtest.a libgtest_main.a <SOURCE_DIR>/include <INSTALL_DIR>/../gtest/)

ExternalProject_Add(metis
DEPENDS metis
PREFIX metis
URL http://glaros.dtc.umn.edu/gkhome/fetch/sw/metis/metis-${Metis_Version}.tar.gz
URL_MD5 0
SOURCE_DIR metis
BUILD_IN_SOURCE 1
BUILD_COMMAND ${CMAKE_BUILD_TOOL}
CONFIGURE_COMMAND ${CMAKE_BUILD_TOOL} -DGKLIB_PATH="Gklib" config shared=1
INSTALL_COMMAND ${CMAKE_BUILD_TOOL} install
INSTALL_DIR <INSTALL_DIR>/../metis
)
