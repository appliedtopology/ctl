include_directories( ${CMAKE_SOURCE_DIR})
add_executable( persistence persistence.cpp)
add_executable( duplicate build_blobs.cpp)
add_executable( build_clique clique_tool.cpp)
