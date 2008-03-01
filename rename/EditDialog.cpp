#include "EditDialog.h"

EditDialog::EditDialog(int index, const QString& path, const QString& left, const QString& right, QWidget *parent) : QWidget(parent), mIndex(index), mPath(path), mLeft(left), mRight(right)
{
	ui.setupUi(this);
	ui.leftLabel->setText(left);
	ui.rightEdit->setText(right);

	setAttribute(Qt::WA_DeleteOnClose);
	setWindowModality(Qt::ApplicationModal);

	connect(ui.okButton, SIGNAL(clicked(bool)), this, SLOT(handleOk()));
	connect(ui.cancelButton, SIGNAL(clicked(bool)), this, SLOT(close()));
};

void EditDialog::handleOk()
{
	if(mRight.compare(ui.rightEdit->text(), Qt::CaseSensitive) != 0)
		emit fileItemUpdated(mIndex, mLeft, ui.rightEdit->text());

	close();
};
