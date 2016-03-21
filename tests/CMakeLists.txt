set(TESTS
test_abstract_simplex.cpp
test_blowup_tool.cpp
test_chain.cpp
test_cover_generation.cpp
test_cube.cpp
test_cubical_chain_complex.cpp
#test_filtration.cpp
test_finite_field.cpp
test_one_skeleton.cpp
test_product_cell.cpp
)
add_executable(ctl_test ctl_test.cpp ${TESTS})
target_link_libraries(ctl_test ${PTHREAD_LIBRARY} ${DSO_FIX})
