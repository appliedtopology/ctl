# Find the native ANN headers and libraries.
#
#  ANN_INCLUDE_DIRS - where to find nana/nana.h, etc.
#  ANN_LIBRARIES    - List of libraries when using nana.
#  ANN_FOUND        - True if nana found.

# Look for the header file.
FIND_PATH(ANN_INCLUDE_DIR NAMES ANN/ANN.h PATHS /usr/include/ /usr/include/ANN /opt/local/include/ANN /usr/local/include/ANN  $ENV{ANN_DIR}/include/)
MARK_AS_ADVANCED(ANN_INCLUDE_DIR)

# Look for the library.
FIND_LIBRARY(ANN_LIBRARY NAMES ANN  PATHS /usr/lib /opt/local/lib  $ENV{ANN_DIR}/lib)
if( NOT ANN_LIBRARY)
FIND_LIBRARY(ANN_LIBRARY NAMES ann  PATHS /usr/lib /opt/local/lib  $ENV{ANN_DIR}/lib)
endif()
set(ANN_LIBRARIES ${ANN_LIBRARY})
MARK_AS_ADVANCED(ANN_LIBRARY ANN_LIBRARIES ANN_LIBRARY_DIR)

# handle the QUIETLY and REQUIRED arguments and set ANN_FOUND to TRUE if 
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(ANN DEFAULT_MSG ANN_LIBRARY ANN_INCLUDE_DIR)

IF(ANN_FOUND)
  SET(ANN_LIBRARIES ${ANN_LIBRARY})
  SET(ANN_INCLUDE_DIRS ${ANN_INCLUDE_DIR})
ELSE(ANN_FOUND)
  SET(ANN_LIBRARIES)
  SET(ANN_INCLUDE_DIRS)
ENDIF(ANN_FOUND)
