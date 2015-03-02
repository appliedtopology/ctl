cmake_minimum_required(VERSION 2.8)
include(ExternalProject)
set( BOOST_PREFIX ../../boost)

ExternalProject_Add(boost
PREFIX            ${BOOST_PREFIX}
TMP_DIR           ${BOOST_PREFIX}/temp
STAMP_DIR         ${BOOST_PREFIX}/stamp
DOWNLOAD_DIR      ${BOOST_PREFIX}/download
URL http://downloads.sourceforge.net/project/boost/boost/${Boost_Version}/boost_${Boost_Version_Underscore}.zip
#--Update/Patch step----------
UPDATE_COMMAND    ""
#--Configure step-------------
SOURCE_DIR        ${BOOST_PREFIX}/source
CONFIGURE_COMMAND ./bootstrap.sh
BUILD_COMMAND     ./b2 install -j 4
                          --build-dir=${BOOST_PREFIX}/build
                          --prefix=${BOOST_PREFIX}
                          threading=multi
BUILD_IN_SOURCE 1
)
