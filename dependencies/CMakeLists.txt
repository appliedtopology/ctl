set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/dependencies/CMakeModules")
set(CMAKE_PREFIX_PATH /usr/lib64/openmpi)

find_package(Boost COMPONENTS program_options serialization REQUIRED)
find_package( MPI REQUIRED) 
find_package( TBB REQUIRED) 
find_package( METIS REQUIRED)
find_package( Doxygen)
find_package( ANN)

include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})

include_directories( ${MPI_INCLUDE_DIR})
link_directories( ${MPI_LIBRARY_DIR})

include_directories(${TBB_INCLUDE_DIR})
link_directories(${TBB_LIBRARY_DIR})
set(TBB_LIBRARIES ${TBB_LIBRARIES} )

include_directories(${METIS_INCLUDE_DIR})
link_directories(${METIS_LIBRARY_DIR})

include_directories(${ANN_INCLUDE_DIR})
link_directories(${ANN_LIBRARY_DIR})



include_directories(${CMAKE_CURRENT_SOURCE_DIR})

if(DOXYGEN_FOUND)
configure_file(${CMAKE_SOURCE_DIR}/doc/Doxyfile.in 
	       ${CMAKE_BINARY_DIR}/Doxyfile @ONLY)
add_custom_target(doc ${DOXYGEN_EXECUTABLE} ${CMAKE_BINARY_DIR}/Doxyfile
WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
COMMENT "Generating API documentation with Doxygen" VERBATIM
)
endif(DOXYGEN_FOUND)
