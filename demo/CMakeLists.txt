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

PROJECT(kawaiidemo)

MESSAGE("** Configuring kawaiidemo **")

INCLUDE_DIRECTORIES(${CHASEN_INCLUDE_DIR} ${LIBKAWAII_INCLUDE_DIR})
LINK_DIRECTORIES(${LIBKAWAII_LINK_DIR})

SET(kawaiidemo_SRCS
	BasicTab.cpp
	kawaiidemo.cpp
	ConversionTab.cpp
	FuriganaTab.cpp
	MainWindow.cpp
	SortTab.cpp
)

SET(kawaiidemo_HDRS
	BasicTab.h
	ConversionTab.h
	FuriganaTab.h
	MainWindow.h
	SortTab.h
)
QT4_WRAP_CPP(kawaiidemo_MOC_SRCS ${kawaiidemo_HDRS})

SET(kawaiidemo_RCC
)
QT4_ADD_RESOURCES(kawaiidemo_RCC_SRCS ${kawaiidemo_RCC})

SET(kawaiidemo_UIS
)
QT4_WRAP_UI(kawaiidemo_UI_SRCS ${kawaiidemo_UIS})

ADD_EXECUTABLE(kawaiidemo ${kawaiidemo_SRCS} ${kawaiidemo_MOC_SRCS} ${kawaiidemo_RCC_SRCS} ${kawaiidemo_UI_SRCS})
TARGET_LINK_LIBRARIES(kawaiidemo -lkawaii ${QT_LIBRARIES} ${CHASEN_LIBRARIES})
INSTALL(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/kawaiidemo DESTINATION bin)
