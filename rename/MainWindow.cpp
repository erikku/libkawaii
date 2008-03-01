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
#include "ListThread.h"

#include "TwinScrollBar.h"
#include "EditDialog.h"

#include <QtCore/QDir>
#include <QtCore/QDate>
#include <QtCore/QSettings>
#include <QtGui/QCloseEvent>
#include <QtGui/QMessageBox>
#include <QtGui/QFileDialog>
#include <QtGui/QDesktopWidget>

#include <kawaii/configure.h>

MainWindow::MainWindow(QWidget *parent, Qt::WindowFlags flags) : QMainWindow(parent, flags), mLocked(false), mDirty(false), mCanRename(false)
{
	QWidget *mainWidget = new QWidget;
	ui.setupUi(mainWidget);

	QAction *action = new QAction("Refresh", this);
	action->setShortcut( QKeySequence("F5") );
	addAction(action);

	connect(action, SIGNAL(triggered()), this, SLOT(refreshList()));
	connect(ui.AboutButton, SIGNAL(clicked(bool)), this, SLOT(showAbout()));
	connect(ui.QuitButton, SIGNAL(clicked(bool)), this, SLOT(close()));
	connect(ui.RefreshButton, SIGNAL(clicked(bool)), this, SLOT(refreshList()));
	connect(ui.WorkingDirectoryBrowse, SIGNAL(clicked(bool)), this, SLOT(browseDir()));
	connect(ui.WorkingDirectoryEdit, SIGNAL(editingFinished()), this, SLOT(refreshList()));
	connect(ui.PreviewSourceList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(editFileName()));
	connect(ui.PreviewTargetList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(editFileName()));

	ui.ReplaceSourceList->setTwin(ui.ReplaceTargetList);
	ui.ReplaceTargetList->setTwin(ui.ReplaceSourceList);
	//ui.ReplaceSourceList->setTwinScrollBar(qobject_cast<TwinScrollBar*>(ui.ReplaceTargetList->verticalScrollBar()));
	//ui.ReplaceTargetList->setTwinScrollBar(qobject_cast<TwinScrollBar*>(ui.ReplaceSourceList->verticalScrollBar()));

	ui.PreviewSourceList->setTwin(ui.PreviewTargetList);
	ui.PreviewTargetList->setTwin(ui.PreviewSourceList);
	//ui.PreviewSourceList->setTwinScrollBar(qobject_cast<TwinScrollBar*>(ui.PreviewTargetList->verticalScrollBar()));
	//ui.PreviewTargetList->setTwinScrollBar(qobject_cast<TwinScrollBar*>(ui.PreviewSourceList->verticalScrollBar()));

	setCentralWidget(mainWidget);
	setWindowTitle(mainWidget->windowTitle());
	resize( QSize(591, 651) );
	loadSettings();

	updateRenameButton();
};

bool MainWindow::isLocked() const
{
	return mLocked;
};

void MainWindow::lock()
{
	mLocked = true;

	ui.OptionsTabWidget->setEnabled(false);
	ui.WorkingDirectoryGroup->setEnabled(false);
	ui.PreviewGroup->setEnabled(false);
	ui.RefreshButton->setEnabled(false);
	ui.RenameButton->setEnabled(false);
};

void MainWindow::unlock()
{
	mLocked = false;

	ui.OptionsTabWidget->setEnabled(true);
	ui.WorkingDirectoryGroup->setEnabled(true);
	ui.PreviewGroup->setEnabled(true);
	ui.RefreshButton->setEnabled(true);
	ui.RenameButton->setEnabled(true);
	updateRenameButton();
};

void MainWindow::loadSettings()
{
	QSettings settings;

	if( settings.contains("General/ReplaceSources") && settings.contains("General/ReplaceTargets") )
	{
		ui.ReplaceSourceList->addItems( settings.value("General/ReplaceSources").toStringList() );
		ui.ReplaceTargetList->addItems( settings.value("General/ReplaceTargets").toStringList() );
	}

	if( settings.contains("General/OptionsTab") )
		ui.OptionsTabWidget->setCurrentIndex( settings.value("General/OptionsTab").toInt() );

	if( settings.contains("General/FilenameSource") )
		ui.FilenameFromTag->setChecked( settings.value("General/FilenameSource").toInt() );

	if( settings.contains("General/CapsOptionEnabled") )
		ui.CapsOptionCheck->setChecked( settings.value("General/CapsOptionEnabled").toBool() );
	if( settings.contains("General/CapsOption") )
		ui.CapsOptionSpinner->setValue( settings.value("General/CapsOption").toInt() );

	if( settings.contains("General/DirPatternEnabled") )
		ui.PatternsDirectoryCheck->setChecked( settings.value("General/DirPatternEnabled").toBool() );

	if( settings.contains("General/DirPattern") )
		ui.PatternsDirectoryEdit->setText( settings.value("General/DirPattern").toString() );
	else
		ui.PatternsDirectoryEdit->setText( tr("[%1] {album}").arg( QDate::currentDate().toString("yyyy.MM.dd") ) );

	if( settings.contains("General/FilenamePattern") )
		ui.PatternsFilenameEdit->setText( settings.value("General/FilenamePattern").toString() );

	if( settings.contains("General/WorkingDir") )
		ui.WorkingDirectoryEdit->setText( settings.value("General/WorkingDir").toString() );
	else
		ui.WorkingDirectoryEdit->setText( QDir::currentPath() );
};

void MainWindow::saveSettings()
{
	QSettings settings;

	QStringList replaceSources;
	for(int i = 0; i < ui.ReplaceSourceList->count(); i++)
		replaceSources << ui.ReplaceSourceList->item(i)->text();

	QStringList replaceTargets;
	for(int i = 0; i < ui.ReplaceTargetList->count(); i++)
		replaceTargets << ui.ReplaceTargetList->item(i)->text();

	settings.setValue("General/ReplaceSources", replaceSources);
	settings.setValue("General/ReplaceTargets", replaceTargets);

	settings.setValue("General/OptionsTab", ui.OptionsTabWidget->currentIndex());

	settings.setValue("General/FilenameSource", ui.FilenameFromTag->isChecked());

	settings.setValue("General/CapsOptionEnabled", ui.CapsOptionCheck->isChecked());
	settings.setValue("General/CapsOption", ui.CapsOptionSpinner->value());

	settings.setValue("General/DirPatternEnabled", ui.PatternsDirectoryCheck->isChecked());
	settings.setValue("General/DirPattern", ui.PatternsDirectoryEdit->text());

	settings.setValue("General/FilenamePattern", ui.PatternsFilenameEdit->text());
	settings.setValue("General/WorkingDir", ui.WorkingDirectoryEdit->text());
};

void MainWindow::refreshList()
{
	if( sender() == ui.WorkingDirectoryEdit && mLastPath == ui.WorkingDirectoryEdit->text() )
		return;

	if( !QDir(ui.WorkingDirectoryEdit->text()).exists() )
	{
		QMessageBox::critical(this, tr("Invalid Directory"), tr("The working directory does not exist. Please select "
			"an existing directory before you refresh the file list."));
	}

	if(mDirty)
	{
		QMessageBox::StandardButton result = QMessageBox::warning(this, tr("Refresh Requested"),
		tr("You are about to refresh the file list. The list has been edited by hand."
			" Are you sure you want to do this?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

		if(result == QMessageBox::No)
			return;
	}

	lock();
	mDirty = false;
	mCanRename = false;

	mLastPath = ui.WorkingDirectoryEdit->text();

	ui.PreviewSourceList->clear();
	ui.PreviewTargetList->clear();

	ListThread *thread = new ListThread;

	connect(thread, SIGNAL(finished()), this, SLOT(unlock()));
	connect(thread, SIGNAL(addFileItem(const QString&, const QString&)), this, SLOT(addFileItem(const QString&, const QString&)));

	thread->setWorkDir( ui.WorkingDirectoryEdit->text() );
	thread->start();
};

void MainWindow::showAbout()
{
	QMessageBox::about(this, tr("About Kawaii Rename"), tr("Kawaii Rename v%1.%2.%3\nCopyright (C) 2007 John Eric Martin <john.eric.martin@gmail.com>"
		"\nLicensed under the GPLv2").arg(LIBKAWAII_VERSION_MAJOR).arg(LIBKAWAII_VERSION_MINOR).arg(LIBKAWAII_VERSION_REVISION));
};

void MainWindow::browseDir()
{
	QString dir = QFileDialog::getExistingDirectory(this, tr("Select Target Directory"));
	if( !dir.isEmpty() )
		ui.WorkingDirectoryEdit->setText(dir);
};

void MainWindow::center()
{
	QRect screenGeometry( qApp->desktop()->screenGeometry( qApp->desktop()->primaryScreen() ) );
	move( ((screenGeometry.width() - width()) / 2)  + screenGeometry.x(), ((screenGeometry.height() - height()) / 2)  + screenGeometry.y() );
};

void MainWindow::addFileItem(const QString& source, const QString& target)
{
	ui.PreviewSourceList->addItem(source);
	ui.PreviewTargetList->addItem(target);

	if( source.compare(target, Qt::CaseSensitive) != 0 )
	{
		mCanRename = true;
		ui.PreviewSourceList->item(ui.PreviewSourceList->count() - 1)->setForeground(QBrush(QColor(Qt::blue)));
		ui.PreviewTargetList->item(ui.PreviewTargetList->count() - 1)->setForeground(QBrush(QColor(Qt::blue)));
	}
};

void MainWindow::updateFileItem(int index, const QString& src, const QString& dst)
{
	ui.PreviewSourceList->item(index)->setText(src);
	ui.PreviewTargetList->item(index)->setText(dst);

	if( src.compare(dst, Qt::CaseSensitive) != 0 )
	{
		ui.PreviewSourceList->item(index)->setForeground(QBrush(QColor(Qt::blue)));
		ui.PreviewTargetList->item(index)->setForeground(QBrush(QColor(Qt::blue)));
	}
	else
	{
		ui.PreviewSourceList->item(index)->setForeground( palette().brush(QPalette::Active, QPalette::Text) );
		ui.PreviewTargetList->item(index)->setForeground( palette().brush(QPalette::Active, QPalette::Text) );
	}
};

void MainWindow::updateRenameButton()
{
	ui.RenameButton->setEnabled(mCanRename);
};

void MainWindow::editFileName()
{
	EditDialog *dialog = new EditDialog( ui.PreviewSourceList->row(ui.PreviewSourceList->selectedItems().first()),
		QDir::toNativeSeparators( QDir::cleanPath(ui.WorkingDirectoryEdit->text() + "/" + ui.PreviewSourceList->selectedItems().first()->text()) ),
		ui.PreviewSourceList->selectedItems().first()->text(), ui.PreviewTargetList->selectedItems().first()->text() );

	connect(dialog, SIGNAL(fileItemUpdated(int, const QString&, const QString&)), this, SLOT(updateFileItem(int, const QString&, const QString&)));
	dialog->show();
};

void MainWindow::closeEvent(QCloseEvent *event)
{
	if(mLocked)
	{
		QMessageBox::StandardButton result = QMessageBox::warning(this, tr("Application About to Close"),
		tr("You are about to close the application. The application is busy processing a request."
			" Are you sure you want to do this?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

		if(result == QMessageBox::No)
		{
			event->ignore();
			return;
		}
	}

	saveSettings();
	QMainWindow::closeEvent(event);
};
