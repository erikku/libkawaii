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

#include "MainWindow.h"

#include <QtGui/QLabel>
#include <QtGui/QGroupBox>
#include <QtGui/QTabWidget>
#include <QtGui/QApplication>
#include <QtGui/QDesktopWidget>

#include "BasicTab.h"
#include "ConversionTab.h"
#include "FuriganaTab.h"
#include "SortTab.h"

#include <kawaii/configure.h>
#include <kawaii/KanaConversion.h>
#include <iostream>

MainWindow::MainWindow(QWidget *parent, Qt::WindowFlags flags) : QMainWindow(parent, flags)
{
	QLabel *welcomeWidget = new QLabel;
	welcomeWidget->setMargin(6);
	welcomeWidget->setWordWrap(true);
	welcomeWidget->setAlignment(Qt::AlignLeft | Qt::AlignTop);
	welcomeWidget->setText( tr("<p>Welcome to Kawaii Demo! Below is a description of the different demos avaliable:</p><ul>"
		"<li><b>Basic</b> - This demo shows the output that chasen produces when given a string. Chasen will take each"
		" scentence component and output it on it's own line. It will end with a line that reads <i>EOS</i> to indicate"
		" the end of the list. As Chasen is a Japanese library, all information is returned in Japanese. Each line consists"
		" of 6 sections, separated by tabs. Any number of sections could be blank.</li>"
		"<li><b>Conversion</b> - This demo converts Japanese between various formats. To space Japanese properly, we need"
		" to know how to split Japanese into words and/or components. Since Chasen anylizes Japanese and gives us detailed"
		" information about each word and/or scentence component, it is ideal for this process.</li>"
		"<li><b>Furigana</b> - This demo expands upon the conversion demo to show how you to create furigana without"
		" asking the user to tell you anything more then the native Japanese scentence. This also shows off a fancy ruby"
		" label widget, because it's just <i>that</i> cool :)</li>"
		"<li><b>Sort</b> - This demo shows how we can expand upon the conversion and furigana demos and use the kana"
		" representation of the words to create an all kana key that can be used to sort a list in a,i,u,e,o order. You can"
		" add words by typing them in the field bellow and pressing enter. If you type in a entry with ruby, it will use"
		" that for a key instead of kana. To remove a entry, double click it.</li></ul>"
		"<center><p>Kawaii Demo v%1<br>Copyright (C) 2007 John Eric Martin &lt;john.eric.martin@gmail.com&gt;<br>Licensed under"
		" the terms of the GPLv2</p></center>").arg(LIBKAWAII_VERSION) );

	QTabWidget *tabWidget = new QTabWidget;
	tabWidget->addTab(welcomeWidget, tr("&Welcome") );
	tabWidget->addTab(new BasicTab, tr("&Basic") );
	tabWidget->addTab(new ConversionTab, tr("&Conversion") );
	tabWidget->addTab(new FuriganaTab, tr("&Furigana") );
	tabWidget->addTab(new SortTab, tr("&Sort") );

	setCentralWidget(tabWidget);
	setWindowTitle( tr("Kawaii Demo v%1").arg(LIBKAWAII_VERSION) );
};

QSize MainWindow::sizeHint() const
{
	return QSize(640, 480);
};

void MainWindow::center()
{
	QRect screenGeometry( qApp->desktop()->screenGeometry( qApp->desktop()->primaryScreen() ) );
	move( ((screenGeometry.width() - width()) / 2)  + screenGeometry.x(), ((screenGeometry.height() - height()) / 2)  + screenGeometry.y() );
};
