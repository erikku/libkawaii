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

#ifndef __SortTab_h__
#define __SortTab_h__

#include <QtCore/QMap>
#include <QtGui/QWidget>

class QLineEdit;
class QListWidget;
class QListWidgetItem;

class SortTab : public QWidget
{
	Q_OBJECT

public:
	SortTab(QWidget *parent = 0);

public slots:
	void addSortItem();
	void removeSortItem(QListWidgetItem *item);
	void refreshLists();

protected:
	QMap<QString, QString> mEntries;

	QLineEdit *mEntryEdit;
	QListWidget *mNormalSort;
	QListWidget *mChasenSort;
};

#endif // __SortTab_h__
