add_executable(ctl_test ctl_test.cpp)
target_link_libraries(ctl_test ${GTEST_LIBRARY} ${PTHREAD_LIBRARY} ${DSO_FIX})
