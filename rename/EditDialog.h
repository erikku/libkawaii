#ifndef __EditDialog_h__
#define __EditDialog_h__

#include "ui_EditDialog.h"

class EditDialog : public QWidget
{
	Q_OBJECT

public:
	EditDialog(int index, const QString& path, const QString& left, const QString& right, QWidget *parent = 0);

public slots:
	void handleOk();

signals:
	void fileItemUpdated(int index, const QString& src, const QString& dst);

protected:
	Ui::EditDialog ui;

	int mIndex;
	QString mPath, mLeft, mRight;
};

#endif // __EditDialog_h__
