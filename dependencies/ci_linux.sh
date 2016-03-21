#!/bin/bash
#we do not support clang on ubuntu 12.x..

sudo yum install tbb-devel metis-devel boost-devel
mkdir build && cd build
cmake ..
make -j4
make run_tests
