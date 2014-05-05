option(ZOOM_PROFILE "Profile code with Zoom profiler" on)

include_directories( /opt/rotateright/Zoom/include/zoom)
link_directories( /opt/rotateright/Zoom/lib
  		  /opt/rotateright/Zoom/lib64)

add_executable(distributed_persistence distributed_persistence_tool.cpp)
target_link_libraries(distributed_persistence ${MPI_LIBRARIES} ${Boost_LIBRARIES})
install( TARGETS distributed_persistence RUNTIME DESTINATION bin)

#add_executable(persistence persistence.cpp)
#install( TARGETS persistence RUNTIME DESTINATION bin)
#
#if( APPLE)
#	target_link_libraries( persistence boost_program_options) 
#	if( ZOOM_PROFILE)
#		target_link_libraries( persistence m RRZoomScript) 
#	endif( ZOOM_PROFILE)
#endif( APPLE)
#if( ${CMAKE_SYSTEM_NAME} MATCHES "Linux")
#	target_link_libraries( persistence boost_program_options)
#	if( ZOOM_PROFILE)
#		target_link_libraries( persistence pthread rt m RRZoomScript)
#	endif( ZOOM_PROFILE)
#endif( ${CMAKE_SYSTEM_NAME} MATCHES "Linux")
#
#add_executable(duplicate build_blobs.cpp)
#install( TARGETS duplicate RUNTIME DESTINATION bin)
#
#add_executable(build_clique clique_tool.cpp)
#install( TARGETS build_clique RUNTIME DESTINATION bin)
#
#add_executable(euler euler_tool.cpp)
#target_link_libraries( euler boost_program_options)
#install( TARGETS euler RUNTIME DESTINATION bin)
#
#add_executable(complex_size complex_size.cpp)
#install( TARGETS complex_size RUNTIME DESTINATION bin)
#
#add_executable (oneskeleton complex_to_graph_tool.cpp)
#target_link_libraries( oneskeleton metis boost_program_options)
#install( TARGETS oneskeleton RUNTIME DESTINATION bin)
#
#add_executable (wghtomet graph_to_metis_tool.cpp)
#install( TARGETS wghtomet RUNTIME DESTINATION bin)
#
#add_executable (metowgh metis_to_graph_tool.cpp)
#install( TARGETS metowgh RUNTIME DESTINATION bin)
#
#add_executable (gpcover gpcover_tool.cpp)
#target_link_libraries( gpcover metis boost_program_options tbb)
#install( TARGETS gpcover RUNTIME DESTINATION bin)
#
#add_executable (concurrent_homology   concurrent_homology_tool)
#target_link_libraries (concurrent_homology boost_program_options tbb metis tbbmalloc  pthread rt m RRZoomScript )
#install( TARGETS concurrent_homology RUNTIME DESTINATION bin)
#
#add_executable (cover_homology   cover_homology_tool)
#target_link_libraries (cover_homology boost_program_options metis tbb pthread rt m RRZoomScript)
#install( TARGETS cover_homology RUNTIME DESTINATION bin)
#
#add_executable (write_filtration write_filtration_tool.cpp)
#target_link_libraries( write_filtration boost_program_options)
#install( TARGETS write_filtration RUNTIME DESTINATION bin)
