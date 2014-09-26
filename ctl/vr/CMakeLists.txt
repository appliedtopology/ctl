add_executable (incremental_test incremental_test.cpp)
target_link_libraries(incremental_test ${Boost_LIBRARIES} ${DSO_FIX})
