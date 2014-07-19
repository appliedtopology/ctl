add_executable( cover_test test_cover_tool.cpp)
target_link_libraries( cover_test ${TBB_LIBRARIES} ${Boost_LIBRARIES} ${METIS_LIBRARIES})
