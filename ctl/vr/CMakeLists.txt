add_executable (incremental_test incremental_test.cpp)
add_executable (inductive_test inductive_test.cpp)
target_link_libraries(incremental_test ${Boost_LIBRARIES} ${DSO_FIX})
target_link_libraries(inductive_test ${Boost_LIBRARIES} ${DSO_FIX})
