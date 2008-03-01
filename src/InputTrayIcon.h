#ifndef __InputTrayIcon_h__
#define __InputTrayIcon_h__

#include <QtGui/QSystemTrayIcon>

class InputTrayIcon : public QSystemTrayIcon
{
	Q_OBJECT

public:
	InputTrayIcon(QObject *parent = 0);
	InputTrayIcon(const QIcon& icon, QObject *parent = 0);

	virtual bool event(QEvent *event);

protected:
	QIcon mSystemIcon;
};

#endif // __InputTrayIcon_h__
