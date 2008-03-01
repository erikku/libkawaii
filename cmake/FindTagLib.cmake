# - Find the taglib includes and library
# This module defines
#  TAGLIB_INCLUDE_DIR, where to find the includes.
#  TAGLIB_LIBRARIES, the libraries required to use taglib.
#  TAGLIB_FOUND, If false, do not try to use taglib.

FIND_PATH(TAGLIB_INCLUDE_DIR taglib.h
  /usr/local/include/taglib
  /usr/include/taglib
  /usr/local/include
  /usr/include
)

FIND_LIBRARY(TAGLIB_LIBRARIES tag
  /usr/local/lib
  /usr/lib
)

SET( TAGLIB_FOUND "NO" )
IF(TAGLIB_INCLUDE_DIR)
  IF(TAGLIB_LIBRARIES)
    SET( TAGLIB_FOUND "YES" )
  ENDIF(TAGLIB_LIBRARIES)
ENDIF(TAGLIB_INCLUDE_DIR)

