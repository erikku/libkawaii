#define QT_STATICPLUGIN

#include "InputContext.h"

#include <iostream>

#include <QtGui/QLabel>
#include <QtGui/QDialog>
#include <QtGui/QListWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QApplication>

Q_EXPORT_PLUGIN2(kawaii_inputcontext, KawaiiInputContextPlugin)

QInputContext* KawaiiInputContextPlugin::create(const QString& key)
{
	if(key == "kawaiimulti")
		return new KawaiiInputContext;

	return 0;
};

QString KawaiiInputContextPlugin::description(const QString& key)
{
	if(key == "kawaiimulti")
		return tr("Input Method Editor to input Japanese text.");

	return QString();
};

QString KawaiiInputContextPlugin::displayName(const QString& key)
{
	if(key == "kawaiimulti")
		return tr("Kawaii Input Method");

	return QString();
};

QStringList KawaiiInputContextPlugin::keys() const
{
	return QStringList() << "kawaiimulti";
};

QStringList KawaiiInputContextPlugin::languages(const QString& key)
{
	if(key == "kawaiimulti")
		return QStringList() << "ja";

	return QStringList();
};

KawaiiInputContext::KawaiiInputContext(QObject *parent) : QInputContext(parent), mCtrlDown(false), mComposing(false)
{
	std::cout << "Hi!" << std::endl;

	QVBoxLayout *imLayout = new QVBoxLayout;
	imLayout->addWidget( new QLabel("Testing") );
	imLayout->addWidget( new QListWidget() );

	imSelection = new QWidget(0, Qt::ToolTip | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
	imSelection->setFocusPolicy(Qt::NoFocus);
	imSelection->setLayout(imLayout);
};

KawaiiInputContext::~KawaiiInputContext()
{
	std::cout << "Bye!" << std::endl;
};

bool KawaiiInputContext::filterEvent(const QEvent *event)
{
	currentWidget = QApplication::focusWidget();

	const QKeyEvent *keyEvent = reinterpret_cast<const QKeyEvent*>(event);

	switch( event->type() )
	{
		case QEvent::KeyPress:
			switch( keyEvent->key() )
			{
				case Qt::Key_Control:
					mCtrlDown = true;
					return true;
				default:
					break;
			}
			break;
		case QEvent::KeyRelease:
			switch( keyEvent->key() )
			{
				case Qt::Key_Control:
					mCtrlDown = false;
					return true;
				case Qt::Key_Space:
					if(mCtrlDown)
					{
						mComposing = !mComposing;
						if(mComposing)
						{
							std::cout << "Showing selection" << std::endl;
							imSelection->setVisible(true);
							//imSelection->show();
							//imSelection->clearFocus();
						}
						return true;
					}
					break;
				default:
					break;
			}
			break;
		default:
			break;
	}
	return mComposing;
};

QString KawaiiInputContext::identifierName()
{
	return "kawaiimulti";
};

QString KawaiiInputContext::language()
{
	return "ja";
};

void KawaiiInputContext::reset()
{
	if(QApplication::focusWidget() == imSelection)
	{
		std::cout << "Focus is on selection, removing focus." << std::endl;
		currentWidget->activateWindow();
		currentWidget->raise();
		return;
	}

	if(mComposing) // We are still composing, so let's store the information for when the widget comes back into focus
	{
		InputInstance instance;
		mInstances[currentWidget] = instance;
	}

	// Update our currentWidget
	currentWidget = QApplication::focusWidget();

	// Restore our information if we have it
	if( mInstances.contains(currentWidget) )
	{
		mComposing = true;
		std::cout << "Reset! Editing is ON" << std::endl;
	}
	else
	{
		mComposing = false;
		//imSelection->hide();

		std::cout << "Reset! Editing is OFF" << std::endl;
	}
};

bool KawaiiInputContext::isComposing() const
{
	return mComposing;
};

