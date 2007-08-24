/******************************************************************************\
*  libkawaii - A Japanese language support library for Qt4                     *
*  Copyright (C) 2007 John Eric Martin <john.eric.martin@gmail.com>            *
*                                                                              *
*  This program is free software; you can redistribute it and/or modify        *
*  it under the terms of the GNU General Public License version 2 as           *
*  published by the Free Software Foundation.                                  *
*                                                                              *
*  This program is distributed in the hope that it will be useful,             *
*  but WITHOUT ANY WARRANTY; without even the implied warranty of              *
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the               *
*  GNU General Public License for more details.                                *
*                                                                              *
*  You should have received a copy of the GNU General Public License           *
*  along with this program; if not, write to the                               *
*  Free Software Foundation, Inc.,                                             *
*  59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.                   *
\******************************************************************************/

#ifndef __MemoryMap_h__
#define __MemoryMap_h__

#include <QtCore/QString>

class MemoryMap
{
public:
	MemoryMap(const QString& path, bool writable = false);
	~MemoryMap();

	char* data();
	char* data() const;
	char* constData() const;

	bool isWritable() const;

	QString path() const;

	int size() const;
	int length() const;

private:
	QString mPath;

	char* mData;
	int mLength;
	bool mWritable;

#if defined(Q_OS_WIN32)
	HANDLE mFile;
	HANDLE mMapFile;
#else
	int mDescriptor;
#endif
};

#endif // __MemoryMap_h__
