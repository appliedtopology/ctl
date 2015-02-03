cmake_minimum_required(VERSION 2.8)
include(ExternalProject)

ExternalProject_Add(boost_build
PREFIX ../../boost
URL http://downloads.sourceforge.net/project/boost/boost/${Boost_Version}/boost_${Boost_Version_Underscore}.zip
URL_MD5 854dcbbff31b896c85c38247060b7713
UPDATE_COMMAND "bootstrap.bat"
CONFIGURE_COMMAND ""
BUILD_COMMAND bjam --with-program_options address-model=64 toolset=msvc-11.0 link=static stage
BUILD_IN_SOURCE 1
INSTALL_COMMAND cp -r -p  <SOURCE_DIR>/lib <SOURCE_DIR>/include <INSTALL_DIR>/../boost/)
)

