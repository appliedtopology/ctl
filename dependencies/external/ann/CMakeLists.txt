cmake_minimum_required(VERSION 2.8)
include(ExternalProject)

if( UNIX)
set(ANN_VAR linux-g++)
endif()

if( Darwin)
set(ANN_VAR macosx-g++)
endif()

ExternalProject_add(ann
PREFIX ../../ann
URL http://www.cs.umd.edu/~mount/ANN/Files/${ANN_Version}/ann_${ANN_Version}.zip
URL_MD5 31267ffbe4e6d04768b3ec21763e9343
CONFIGURE_COMMAND ""
BUILD_COMMAND make ${ANN_VAR}
BUILD_IN_SOURCE 1
INSTALL_COMMAND cp -r -p <SOURCE_DIR>/lib <SOURCE_DIR>/include <INSTALL_DIR>/../ann/
)

