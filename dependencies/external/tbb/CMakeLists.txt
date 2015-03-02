cmake_minimum_required(VERSION 2.8)
include(ExternalProject)
set( TBB_PREFIX ../../tbb)

if( Darwin)
set(TBB_VAR compiler=clang stdlib=libc++)
endif()

ExternalProject_Add(tbb
PREFIX            ${TBB_PREFIX}
TMP_DIR           ${TBB_PREFIX}/temp
STAMP_DIR         ${TBB_PREFIX}/stamp
DOWNLOAD_DIR      ${TBB_PREFIX}/download
URL http://www.threadingbuildingblocks.org/sites/default/files/software_releases/source/tbb${TBB_Version}_src.tgz
SOURCE_DIR        ${TBB_PREFIX}/source
BUILD_COMMAND     ./make -j ${TBB_VAR}
BUILD_IN_SOURCE 1
INSTALL_COMMAND cp -r -p <SOURCE_DIR>/build <SOURCE_DIR>/include <INSTALL_DIR>/../tbb/
)
