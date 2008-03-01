#include "InputTrayIcon.h"

#include <QtGui/QMenu>
#include <QtGui/QMessageBox>
#include <QtGui/QApplication>
#include <QtGui/QInputContext>
#include <iostream>

InputTrayIcon::InputTrayIcon(QObject *parent) : QSystemTrayIcon(parent)
{
	mSystemIcon = QIcon(":/system.png");
	setIcon(mSystemIcon);

	QMenu *inputContextMenu = new QMenu;
	inputContextMenu->addAction(mSystemIcon, tr("System"));

	setContextMenu(inputContextMenu);
	setToolTip( tr("Current Input Method: %1").arg(tr("System")) );

	std::cout << qApp->inputContext()->identifierName().toLocal8Bit().data() << std::endl;

	if( !qApp->inputContext() || (qApp->inputContext() && qApp->inputContext()->identifierName() != "kawaiimulti") )
	{
		QMessageBox::warning( 0, tr("KawaiiMultiInputContext Not Loaded"), tr("The KawaiiMultiInputContext is not loaded. "
			"Please update your code to load this input context to make use of the system tray icon") );

		deleteLater();
	}
};

InputTrayIcon::InputTrayIcon(const QIcon& icon, QObject *parent) : QSystemTrayIcon(icon, parent)
{
	mSystemIcon = icon;

	if( !qApp->inputContext() || (qApp->inputContext() && qApp->inputContext()->identifierName() != "kawaiimulti") )
	{
		QMessageBox::warning( 0, tr("KawaiiMultiInputContext Not Loaded"), tr("The KawaiiMultiInputContext is not loaded. "
			"Please update your code to load this input context to make use of the system tray icon") );

		deleteLater();
	}
};

bool InputTrayIcon::event(QEvent *event)
{
	if(event->type() == QEvent::Wheel)
	{
		if(reinterpret_cast<QWheelEvent*>(event)->delta() < 0)
			std::cout << "down" << std::endl;
		else
			std::cout << "up" << std::endl;
	}

	if(event->type() == QEvent::MouseButtonRelease)
	{
		Qt::MouseButton button = reinterpret_cast<QMouseEvent*>(event)->button();

		if(button == Qt::LeftButton || button == Qt::RightButton)
		{
			std::cout << "popup" << std::endl;
			contextMenu()->popup( reinterpret_cast<QMouseEvent*>(event)->globalPos() );
			return true;
		}
	}

	return QSystemTrayIcon::event(event);
};
