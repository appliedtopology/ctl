cmake_minimum_required(VERSION 2.8)
include(ExternalProject)

ExternalProject_Add(gtest_build
PREFIX ../../gtest 
URL http://googletest.googlecode.com/files/gtest-1.7.0.zip
INSTALL_COMMAND cp -r -p libgtest.a libgtest_main.a <SOURCE_DIR>/include <INSTALL_DIR>/../gtest/)
