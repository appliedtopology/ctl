if( Boost_FOUND)
add_executable(persistence persistence.cpp)
install( TARGETS persistence RUNTIME DESTINATION bin)

if( APPLE)
	target_link_libraries( persistence ${Boost_LIBRARIES} ${DSO_FIX}) 
endif( APPLE)
if( ${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	target_link_libraries( persistence ${Boost_LIBRARIES} ${DSO_FIX})
endif( ${CMAKE_SYSTEM_NAME} MATCHES "Linux")
endif()

if( ANN_FOUND)
add_executable(ngraph ngraph.cpp)
target_link_libraries( ngraph ${ANN_LIBRARIES})
install( TARGETS ngraph RUNTIME DESTINATION bin)
endif()

add_executable(duplicate build_blobs.cpp)
install( TARGETS duplicate RUNTIME DESTINATION bin)

add_executable(build_clique clique_tool.cpp)
install( TARGETS build_clique RUNTIME DESTINATION bin)

if( Boost_FOUND)
add_executable(euler euler_tool.cpp)
target_link_libraries( euler ${Boost_LIBRARY_PATH} ${Boost_LIBRARIES} ${DSO_FIX})
install( TARGETS euler RUNTIME DESTINATION bin)
endif()

add_executable(complex_size complex_size.cpp)
install( TARGETS complex_size RUNTIME DESTINATION bin)

if( METIS_FOUND AND Boost_FOUND)
add_executable (oneskeleton complex_to_graph_tool.cpp)
target_link_libraries( oneskeleton ${METIS_LIBRARIES} ${Boost_LIBRARIES} ${DSO_FIX})
install( TARGETS oneskeleton RUNTIME DESTINATION bin)
endif()

add_executable (wghtomet graph_to_metis_tool.cpp)
install( TARGETS wghtomet RUNTIME DESTINATION bin)

add_executable (metowgh metis_to_graph_tool.cpp)
install( TARGETS metowgh RUNTIME DESTINATION bin)

if( METIS_FOUND AND Boost_FOUND)
add_executable (gpcover gpcover_tool.cpp)
target_link_libraries( gpcover ${METIS_LIBRARIES} ${Boost_LIBRARIES} ${TBB_LIBRARIES} ${DSO_FIX})
install( TARGETS gpcover RUNTIME DESTINATION bin)
endif()

if( Boost_FOUND)
add_executable (ctl2phat phat_tool.cpp)
target_link_libraries( ctl2phat ${Boost_LIBRARIES})
install( TARGETS ctl2phat RUNTIME DESTINATION bin)
endif()

if( Boost_FOUND AND METIS_FOUND AND TBB_FOUND)
add_executable (concurrent_homology   concurrent_homology_tool)
target_link_libraries (concurrent_homology ${Boost_LIBRARY_PATH}
${TBB_LIBRARY_PATH} ${Boost_LIBRARIES} ${TBB_LIBRARIES} ${METIS_LIBRARIES} ${DSO_FIX})
install( TARGETS concurrent_homology RUNTIME DESTINATION bin)
endif()

if( Boost_FOUND AND METIS_FOUND AND TBB_FOUND)
add_executable (cover_homology   cover_homology_tool)
target_link_libraries (cover_homology ${Boost_LIBRARIES} ${METIS_LIBRARIES} ${TBB_LIBRARIES} ${DSO_FIX})
install( TARGETS cover_homology RUNTIME DESTINATION bin)
endif()

if( Boost_FOUND AND ANN_FOUND)
add_executable (vr vr.cpp)
target_link_libraries( vr ${Boost_LIBRARIES} ${ANN_LIBRARIES} ${DSO_FIX})
endif()

if (Boost_FOUND)
add_executable (write_filtration write_filtration_tool.cpp)
target_link_libraries( write_filtration ${Boost_LIBRARIES} ${DSO_FIX})
install( TARGETS write_filtration RUNTIME DESTINATION bin)
endif()
