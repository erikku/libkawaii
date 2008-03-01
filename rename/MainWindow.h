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

#include <QtGui/QMainWindow>

#include "ui_MainWindow.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0, Qt::WindowFlags flags = 0);

	bool isLocked() const;

public slots:
	void lock();
	void unlock();

	void loadSettings();
	void saveSettings();
	void refreshList();
	void showAbout();
	void browseDir();
	void center();

	void addFileItem(const QString& source, const QString& target);
	void updateFileItem(int index, const QString& src, const QString& dst);
	void updateRenameButton();
	void editFileName();

protected:
	virtual void closeEvent(QCloseEvent *event);

	QString mLastPath;

	bool mDirty;
	bool mLocked;
	bool mCanRename;
	Ui::MainWindow ui;
};
