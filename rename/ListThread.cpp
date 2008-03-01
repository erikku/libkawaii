#include "ListThread.h"

#include <QtCore/QDir>
#include <QtCore/QFileInfo>

#include <kawaii/ChasenUtils.h>
#include <iostream>

void ListThread::handleFiles(const QString& base, const QString& pre)
{
	QDir dir(base);

	QStringList dirs = dir.entryList(QStringList(), QDir::AllDirs | QDir::NoDotAndDotDot, QDir::Name);
	foreach(QString path, dirs)
		handleFiles(QDir::cleanPath(base + "/" + path), QDir::cleanPath(pre + (pre.isEmpty() ? QString() : "/") + path));

	QStringList files = dir.entryList(QStringList(), QDir::Files, QDir::Name);
	foreach(QString path, files)
	{
		QString new_path = chasenRomaji(QFileInfo(path).baseName(), true) + '.' + QFileInfo(path).completeSuffix();
		if( new_path.compare(path, Qt::CaseSensitive) != 0 )
			new_path = new_path.toLower();

		QString filePath = pre + (pre.isEmpty() ? QString() : "/");
		emit addFileItem(QDir::cleanPath(filePath + path), QDir::cleanPath(filePath + new_path));
	}
};

void ListThread::setWorkDir(const QString& dir)
{
	mWorkDir = dir;
};

void ListThread::run()
{
	handleFiles(mWorkDir);
};
