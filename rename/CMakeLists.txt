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

PROJECT(kawaiirename)

MESSAGE("** Configuring kawaiirename **")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${CHASEN_INCLUDE_DIR} ${TAGLIB_INCLUDE_DIR} ${LIBKAWAII_INCLUDE_DIR})
LINK_DIRECTORIES(${LIBKAWAII_LINK_DIR})

SET(kawaiirename_SRCS
	EditDialog.cpp
	ListThread.cpp
	MainWindow.cpp
	TwinList.cpp
	TwinScrollBar.cpp
	kawaiirename.cpp
)

SET(kawaiirename_HDRS
	EditDialog.h
	ListThread.h
	TwinList.h
	TwinScrollBar.h
	MainWindow.h
)
QT4_WRAP_CPP(kawaiirename_MOC_SRCS ${kawaiirename_HDRS})

SET(kawaiirename_RCC
	resources.qrc
)
QT4_ADD_RESOURCES(kawaiirename_RCC_SRCS ${kawaiirename_RCC})

SET(kawaiirename_UIS
	EditDialog.ui
	MainWindow.ui
)
QT4_WRAP_UI(kawaiirename_UI_SRCS ${kawaiirename_UIS})

ADD_EXECUTABLE(kawaiirename ${kawaiirename_SRCS} ${kawaiirename_MOC_SRCS} ${kawaiirename_RCC_SRCS} ${kawaiirename_UI_SRCS})

IF(MINGW)
	SET_TARGET_PROPERTIES(kawaiirename PROPERTIES LINK_FLAGS -mwindows)
ENDIF(MINGW)

TARGET_LINK_LIBRARIES(kawaiirename -lkawaii ${QT_LIBRARIES} ${CHASEN_LIBRARIES} ${TAGLIB_LIBRARIES})

INSTALL(TARGETS kawaiirename DESTINATION bin)
