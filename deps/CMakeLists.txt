set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/deps/CMakeModules")
set(CMAKE_PREFIX_PATH /usr/lib64/openmpi)

include(ExternalProject)
include( ${CMAKE_SOURCE_DIR}/build_dependencies.txt RESULT_VARIABLE)
#if( RESULT_VARIABLE)
##TODO: Check if not found
#endif()

if( NOT BOOST_FOUND)
	message("Boost not specified by user config. Falling back to CMake find_package")
	find_package(Boost COMPONENTS program_options serialization mpi)
	if (NOT BOOST_FOUND)

	endif()	
endif()

if( NOT MPI_FOUND)
	message( "MPI Not Specified in User Config. Falling back to CMake find_package")
	find_package(MPI REQUIRED)
endif()

if( NOT TBB_FOUND)
	message( "TBB Not Specified in User Config. Falling back to CMake find_package")
	find_package(TBB) 
endif()

if( NOT ANN_FOUND)
	message( "ANN Not Specified in User Config. Falling back to CMake find_package")
	find_package(ANN)
	
endif()
if( NOT METIS_FOUND)
	message( "METIS Not Specified in User Config. Falling back to CMake find_package")
	find_package(METIS) 
endif()

if( NOT DOXYGEN_FOUND)
	message( "Doxygen Not Specified in User Config. Falling back to CMake find_package")
	find_package(Doxygen)
endif() 

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

ExternalProject_Add(gtest PREFIX gtest URL http://googletest.googlecode.com/files/gtest-1.7.0.zip
  INSTALL_COMMAND cp -r -p libgtest.a libgtest_main.a <SOURCE_DIR>/include <INSTALL_DIR>/../gtest/)
