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

PROJECT(kawaii)

CMAKE_MINIMUM_REQUIRED(VERSION 2.4.5)

OPTION(BUILD_DOCS "Build the documentation" OFF)
OPTION(BUILD_KAWAIIDEMO "Build the Kawaii Demo application" ON)
OPTION(BUILD_KAWAIIRENAME "Build the Kawaii Rename application" ON)

SET(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

#
# DEPENDENCIES
#
MESSAGE("** Checking Dependencies **")
INCLUDE(UsePkgConfig)

# Qt 4.3.x
#SET(QT_USE_QTSQL TRUE)
#SET(QT_USE_QTXML TRUE)
FIND_PACKAGE(Qt4 REQUIRED)
INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})

# chasen
INCLUDE(FindChasen)
IF(CHASEN_FOUND)
	MESSAGE("-- Found chasen")
ELSE(CHASEN_FOUND)
	MESSAGE(FATAL_ERROR "chasen not found! You need to fix this!")
ENDIF(CHASEN_FOUND)

IF(BUILD_KAWAIIRENAME)
	INCLUDE(FindTagLib)
	IF(TAGLIB_FOUND)
		MESSAGE("-- Found taglib")
	ELSE(TAGLIB_FOUND)
		MESSAGE(FATAL_ERROR "taglib not found! You need to fix this!")
	ENDIF(TAGLIB_FOUND)
ENDIF(BUILD_KAWAIIRENAME)

#
# SETUP LIBRARY COMPILE PATHS
#
SET(LIBKAWAII_INCLUDE_DIR ${CMAKE_BINARY_DIR}/include)
SET(LIBKAWAII_LINK_DIR ${CMAKE_BINARY_DIR}/src)
SET(LIBKAWAII_LINK_FLAGS -L${LIBKAWAII_LINK_DIR}/ -lkawaii)
SET(LIBKAWAII_CFLAGS -I${LIBKAWAII_INCLUDE_DIR})

ADD_SUBDIRECTORY(src)

IF(BUILD_KAWAIIDEMO)
	ADD_SUBDIRECTORY(demo)
ENDIF(BUILD_KAWAIIDEMO)
IF(BUILD_KAWAIIRENAME)
	ADD_SUBDIRECTORY(rename)
ENDIF(BUILD_KAWAIIRENAME)
