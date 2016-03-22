add_executable(ctl_test ctl_test.cpp)
target_link_libraries(ctl_test ${PTHREAD_LIBRARY} ${DSO_FIX})
