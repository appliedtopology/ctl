cmake_minimum_required(VERSION 2.8)
include(ExternalProject)

ExternalProject_Add(metis_build
PREFIX ../../metis
URL http://glaros.dtc.umn.edu/gkhome/fetch/sw/metis/metis-${Metis_Version}.tar.gz
BUILD_IN_SOURCE 1
CONFIGURE_COMMAND cmake -DSHARED=True -DGKLIB_PATH=GKlib 
BUILD_COMMAND make -j
INSTALL_COMMAND cp -r -p <SOURCE_DIR>/libmetis <SOURCE_DIR>/include <INSTALL_DIR>/../metis)
