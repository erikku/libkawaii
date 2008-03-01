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

PROJECT(libkawaii)

MESSAGE("** Configuring libkawaii **")

SET(LIBKAWAII_OFFICIAL true)
SET(LIBKAWAII_REVISION 0)
SET(LIBKAWAII_VERSION 0.9.1)
SET(LIBKAWAII_VERSION_MAJOR 0)
SET(LIBKAWAII_VERSION_MINOR 9)
SET(LIBKAWAII_VERSION_REVISION 1)
SET(LIBKAWAII_VERSION_SVNREVISION 0)
SET(LIBKAWAII_SANE_VERSION ${LIBKAWAII_VERSION_MAJOR}.${LIBKAWAII_VERSION_MINOR}.${LIBKAWAII_VERSION_REVISION})

#INCLUDE(FindSubversion)
#IF(Subversion_FOUND)
#	FIND_PATH(IS_SVN_CHECKOUT entries ${CMAKE_SOURCE_DIR}/.svn NO_DEFAULT_PATH)
#	IF(IS_SVN_CHECKOUT)
#		Subversion_WC_INFO(${PROJECT_SOURCE_DIR} Project)
#		SET(LIBKAWAII_OFFICIAL false)
#		SET(LIBKAWAII_SVNREVISION ${Project_WC_REVISION})
#		SET(LIBKAWAII_VERSION ${LIBKAWAII_VERSION}-SVN${LIBKAWAII_SVNREVISION})
#	ENDIF(IS_SVN_CHECKOUT)
#ENDIF(Subversion_FOUND)

INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR}/include/kawaii ${CHASEN_INCLUDE_DIR})

CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/configure.h.in ${CMAKE_BINARY_DIR}/include/kawaii/configure.h @ONLY IMMEDIATE)
CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/scripts/libkawaii.pc.in ${CMAKE_CURRENT_BINARY_DIR}/libkawaii.pc @ONLY IMMEDIATE)
CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/scripts/chasenrc.in ${CMAKE_BINARY_DIR}/chasenrc @ONLY)

SET(libkawaii_SRCS
	ChasenUtils.cpp
	InputContext.cpp
	InputTrayIcon.cpp
	KanaConversion.cpp
	Label.cpp
	LineEdit.cpp
	#MemoryMap.cpp
)

SET(libkawaii_HDRS
	InputContext.h
	InputTrayIcon.h
	Label.h
	LineEdit.h
)

SET(libkawaii_HDRS_NOMOC
	ChasenUtils.h
	KanaConversion.h
)

SET(libkawaii_RCC
    ../res/libkawaii.qrc
)

FILE(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/include)
FILE(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/include/kawaii)

FOREACH(current_header ${libkawaii_HDRS} ${libkawaii_HDRS_NOMOC})
	CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/${current_header} ${CMAKE_BINARY_DIR}/include/kawaii/${current_header} COPYONLY)
ENDFOREACH(current_header)

QT4_WRAP_CPP(libkawaii_SRCS ${libkawaii_HDRS})

QT4_ADD_RESOURCES(libkawaii_SRCS ${libkawaii_RCC})

ADD_LIBRARY(kawaii SHARED ${libkawaii_SRCS})

SET_TARGET_PROPERTIES(kawaii PROPERTIES VERSION ${LIBKAWAII_SANE_VERSION} SOVERSION ${LIBKAWAII_VERSION_MAJOR})

IF(MINGW)
	TARGET_LINK_LIBRARIES(kawaii ${QT_LIBRARIES} ${CHASEN_LIBRARIES} -liconv)
ELSE(MINGW)
	TARGET_LINK_LIBRARIES(kawaii ${QT_LIBRARIES} ${CHASEN_LIBRARIES})
ENDIF(MINGW)

# Doxygen Docs
SET(DOXYGEN_LANG English)

IF(BUILD_DOCS)
	INCLUDE(FindDoxygen)
	MAKE_DIRECTORY(${CMAKE_BINARY_DIR}/doc)
	MAKE_DIRECTORY(${CMAKE_BINARY_DIR}/doc/api)
	MAKE_DIRECTORY(${CMAKE_BINARY_DIR}/doc/api/html)

	SET(Doxyfile_in ${CMAKE_SOURCE_DIR}/scripts/Doxyfile.in)
	SET(Doxyfile ${CMAKE_BINARY_DIR}/Doxyfile)
	SET(Doxydepends ${Doxyfile_in} ${libkawaii_SRCS} ${libkawaii_HDRS} ${libkawaii_HDRS_NOMOC} ${CMAKE_SOURCE_DIR}/doc/Mainpage.dox)

	CONFIGURE_FILE(${Doxyfile_in} ${Doxyfile} @ONLY)

	ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_BINARY_DIR}/doc/api/libkawaii.tag COMMAND ${DOXYGEN} ${CMAKE_BINARY_DIR}/Doxyfile > ${CMAKE_BINARY_DIR}/doxygen.log DEPENDS ${Doxydepends} COMMENT "Generating API Documentation" VERBATIM)
	ADD_CUSTOM_TARGET(docs ALL DEPENDS ${Doxydepends} ${CMAKE_BINARY_DIR}/doc/api/libkawaii.tag)

	INSTALL(FILES ${CMAKE_BINARY_DIR}/doc/api/libkawaii.tag DESTINATION share/libkawaii/doc/api)
	INSTALL(DIRECTORY ${CMAKE_BINARY_DIR}/doc/api/html DESTINATION share/libkawaii/doc/api)

	MESSAGE("** API Documentation will be built **")

	ADD_DEPENDENCIES(kawaii docs)
ENDIF(BUILD_DOCS)

INSTALL(TARGETS kawaii DESTINATION lib)
INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/libkawaii.pc DESTINATION lib/pkgconfig)
INSTALL(DIRECTORY ${CMAKE_SOURCE_DIR}/ipadic DESTINATION share/libkawaii)
INSTALL(FILES ${CMAKE_BINARY_DIR}/chasenrc DESTINATION share/libkawaii)
INSTALL(FILES ${libkawaii_HDRS} ${libkawaii_HDRS_NOMOC} ${CMAKE_BINARY_DIR}/include/kawaii/configure.h DESTINATION include/kawaii)
