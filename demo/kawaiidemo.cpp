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

#include <QtGui/QApplication>

#include "MainWindow.h"

#include <QtGui/QInputContextFactory>

#include <kawaii/InputContext.h>
#include <kawaii/InputTrayIcon.h>

#include <iostream>

Q_IMPORT_PLUGIN(kawaii_inputcontext)

int main(int argc, char **argv)
{
	QApplication app(argc, argv);

	QInputContext *ctx = QInputContextFactory::create("kawaiimulti", 0);
	if(ctx)
	{
		app.setInputContext(ctx);
		if(app.inputContext() != ctx)
			std::cout << "WTF!" << std::endl;
	}
	else
	{
		std::cout << "Error!" << std::endl;
		foreach(QString key, QInputContextFactory::keys())
			std::cout << key.toLocal8Bit().data() << " ";
		std::cout << std::endl;
	}

	InputTrayIcon *tray = new InputTrayIcon;
	tray->show();

	MainWindow *window = new MainWindow;
	window->show();
	window->center();

	return app.exec();
};

