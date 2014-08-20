# - Find nana
# Find the native ANN headers and libraries.
#
#  ANN_INCLUDE_DIRS - where to find nana/nana.h, etc.
#  ANN_LIBRARIES    - List of libraries when using nana.
#  ANN_FOUND        - True if nana found.

# Look for the header file.
FIND_PATH(ANN_INCLUDE_DIR NAMES ANN/ANN.h)
MARK_AS_ADVANCED(ANN_INCLUDE_DIR)

# Look for the library.
FIND_LIBRARY(ANN_LIBRARY NAMES ANN)
MARK_AS_ADVANCED(ANN_LIBRARY)

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
