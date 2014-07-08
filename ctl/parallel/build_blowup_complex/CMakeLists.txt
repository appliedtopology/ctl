add_executable (blowup_test  test_blowup_tool)

# Linking to project libraries for executable
target_link_libraries (blowup_test ${Boost_LIBRARIES} ${TBB_LIBRARIES} ${METIS_LIBRARIES} ${DSO_FIXES})
