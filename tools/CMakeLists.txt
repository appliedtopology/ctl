if( ${CMAKE_SYSTEM_NAME} MATCHES "Linux")
include_directories( /opt/rotateright/Zoom/include/zoom)
link_directories( /opt/rotateright/Zoom/lib
  		  /opt/rotateright/Zoom/lib64)
endif( ${CMAKE_SYSTEM_NAME} MATCHES "Linux")
 
if( APPLE)
include_directories( /Library/RotateRight/Zoom/include/zoom)
link_directories( /Library/RotateRight/Zoom/lib64)
endif( APPLE)
include_directories( ${CMAKE_SOURCE_DIR}  
  		     ${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR}) 

add_executable( persistence persistence.cpp)
target_link_libraries( persistence m RRZoomScript boost_program_options)
add_executable( duplicate build_blobs.cpp)
add_executable( build_clique clique_tool.cpp)
add_executable( euler euler_tool.cpp)
target_link_libraries( euler boost_program_options)
