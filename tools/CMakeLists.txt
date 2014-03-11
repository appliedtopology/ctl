include_directories( 
  /opt/rotateright/Zoom/include/zoom
  ${CMAKE_SOURCE_DIR}  
  ${Boost_INCLUDE_DIR}
)
link_directories(${Boost_LIBRARY_DIR} 
  /opt/rotateright/Zoom/lib
  /opt/rotateright/Zoom/lib64
)

add_executable( persistence persistence.cpp)
target_link_libraries( persistence rt m RRZoomScript boost_program_options)
add_executable( duplicate build_blobs.cpp)
add_executable( build_clique clique_tool.cpp)
add_executable( euler euler_tool.cpp)
target_link_libraries( euler boost_program_options)
