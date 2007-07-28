#  libkawaii - A Japanese language support library for Qt4
#  Copyright (C) 2007 John Eric Martin <john.eric.martin@gmail.com>
#
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License version 2 as
#  published by the Free Software Foundation.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the
#  Free Software Foundation, Inc.,
#  59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

# - Find the chasen includes and library
# This module defines
#  CHASEN_INCLUDE_DIR, where to find the includes.
#  CHASEN_LIBRARIES, the libraries required to use chasen.
#  CHASEN_FOUND, If false, do not try to use chasen.

FIND_PATH(CHASEN_INCLUDE_DIR chasen.h
  /usr/local/include
  /usr/include
)

FIND_LIBRARY(CHASEN_LIBRARIES chasen
  /usr/local/lib
  /usr/lib
)

SET( CHASEN_FOUND "NO" )
IF(CHASEN_INCLUDE_DIR)
  IF(CHASEN_LIBRARIES)
    SET( CHASEN_FOUND "YES" )
  ENDIF(CHASEN_LIBRARIES)
ENDIF(CHASEN_INCLUDE_DIR)
