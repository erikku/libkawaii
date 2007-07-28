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

#include "LineEdit.h"
#include "KanaConversion.h"

#include <QtGui/QKeyEvent>

KawaiiLineEdit::KawaiiLineEdit(QWidget *parent) : QLineEdit(parent), mRomajiMode(true)
{
	connect(this, SIGNAL(selectionChanged()), this, SLOT(updateCursor()));
	connect(this, SIGNAL(cursorPositionChanged(int, int)), this, SLOT(updateCursor()));
	connect(this, SIGNAL(textEdited(const QString&)), this, SLOT(updateText()));
};

KawaiiLineEdit::KawaiiLineEdit(const QString& contents, QWidget *parent) : QLineEdit(parent), mRomajiMode(true)
{
	connect(this, SIGNAL(selectionChanged()), this, SLOT(updateCursor()));
	connect(this, SIGNAL(cursorPositionChanged(int, int)), this, SLOT(updateCursor()));
	connect(this, SIGNAL(textChanged(const QString&)), this, SLOT(updateText()));
};

void KawaiiLineEdit::updateCursor()
{
	if(!mRomajiMode)
		return;

	deselect();
	setCursorPosition( text().length() );
};

bool KawaiiLineEdit::romajiMode()
{
	return mRomajiMode;
};

void KawaiiLineEdit::setRomajiMode(bool enabled)
{
	bool block = blockSignals(true);

	if(enabled)
	{
		// If we are switching, process the text already in the box
		if(!mRomajiMode)
		{
			mRealText.clear();
			mLastText.clear();
		}
	}
	else
	{
		// If we are switching from romaji mode, move the real text into the box
		if(mRomajiMode)
		{
			setText(mRealText);
			mRealText.clear();
			mLastText.clear();

		}
	}

	blockSignals(block);

	mRomajiMode = enabled;
	updateCursor();
	updateText();
};

void KawaiiLineEdit::updateText()
{
	if(!mRomajiMode)
		return;

	bool block = blockSignals(true);

	mRealText += text().replace(mLastText, QString());
	setText( romajiToKana(mRealText) );
	mLastText = text();

	blockSignals(block);
};

void KawaiiLineEdit::keyPressEvent(QKeyEvent *event)
{
	if(mRomajiMode && event->key() == Qt::Key_Backspace)
	{
		// Remove the last character in the string (if there is one)
		mRealText = (mRealText.length() > 0) ? mRealText.left( mRealText.length() - 1 ) : QString();
		event->accept();
		updateText();

		return;
	}

	QLineEdit::keyPressEvent(event);
};
