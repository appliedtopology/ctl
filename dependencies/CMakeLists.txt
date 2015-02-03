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

if( GTest_FOUND)
   set(GTEST_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/dependencies/gtest/include)
   set(GTEST_LIBRARY_DIR ${CMAKE_SOURCE_DIR}/dependencies/gtest)
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

add_custom_target(ann COMMAND cmake . -DANN_Version=${ANN_Version} 
		      COMMAND make WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/external/ann)
add_custom_target(boost COMMAND cmake . -DBoost_Version_Underscore=${Boost_Version_Underscore} -DBoost_Version=${Boost_Version} 
		        COMMAND make WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/external/boost)
add_custom_target(metis COMMAND cmake . -DMetis_Version=${Metis_Version} COMMAND make WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/external/metis)
add_custom_target(gtest COMMAND cmake . COMMAND make WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/external/gtest)
