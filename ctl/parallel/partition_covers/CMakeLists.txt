add_executable( cover_test test_cover_tool.cpp)
target_link_libraries( cover_test tbb boost_program_options metis)
