#ifndef __ListThread_h__
#define __ListThread_h__

#include <QtCore/QThread>

class ListThread : public QThread
{
	Q_OBJECT

public:
	void setWorkDir(const QString& dir);

signals:
	void addFileItem(const QString& source, const QString& target);

protected:
	virtual void run();

	void handleFiles(const QString& base, const QString& pre = QString());

	QString mWorkDir;
};

#endif // __ListThread_h__
