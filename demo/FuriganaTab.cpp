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

#include "FuriganaTab.h"

#include <QtGui/QLabel>
#include <QtGui/QTextEdit>
#include <QtGui/QComboBox>
#include <QtGui/QGridLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QMessageBox>

#include <kawaii/ChasenUtils.h>
#include <kawaii/Label.h>
#include <iostream>

FuriganaTab::FuriganaTab(QWidget *parent) : QWidget(parent)
{
	mRubyFormats[ tr("Standard Ruby") ] = Normal;
	mRubyFormats[ tr("Flat (Ruby in Parentheses)") ] = Flat;
	mRubyFormats[ tr("Only the Kanji (Bottom Portion)") ] = Bottom;
	mRubyFormats[ tr("Only the Furigana (Top Portion)") ] = Top;
	mRubyFormats[ tr("Short Hand (e.g. {bottom|top})") ] = ShortHand;
	mRubyFormats[ tr("Wiki (e.g. {{ruby|bottom|top}})") ] = Wiki;
	mRubyFormats[ tr("Standard Ruby w/o Parentheses (<rp> tags)") ] = NoParentheses;

	QLabel *description = new QLabel;
	description->setWordWrap(true);
	description->setAlignment(Qt::AlignLeft | Qt::AlignTop);
	description->setText( tr("<p>This demo expands upon the conversion demo to show how you to create furigana without"
		" asking the user to tell you anything more then the native Japanese scentence. This also shows off a fancy ruby"
		" label widget, because it's just <i>that</i> cool :)</p>") );

	QLabel *entryLabel = new QLabel( tr("Text:") );
	entryLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);

	mEntryEdit = new QTextEdit;
	mEntryEdit->setAcceptRichText(false);
	mEntryEdit->setToolTip( tr("Enter the text you want to make furigana here.") );

	// Ruby Combo
	QLabel *rubyLabel = new QLabel( tr("Ruby:") );
	mRubyCombo = new QComboBox;
	mRubyCombo->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
	mRubyCombo->addItem( tr("Leave as is") );
	mRubyCombo->addItems( mRubyFormats.keys() );
	mRubyCombo->setToolTip( tr("Select how to convert ruby tags here.") );

	// Convert Result
	QLabel *resultLabel = new QLabel( tr("Result:") );
	resultLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);
	mConvertResult = new QTextEdit;
	mConvertResult->setAcceptRichText(false);
	//mConvertResult->setReadOnly(true);
	mConvertResult->setToolTip( tr("The text that has been converted ends up here.") );

	QFont topFont = font();
	topFont.setFamily("mikachan");
	topFont.setPointSize(6);
	topFont.setUnderline(true);
	QFont bottomFont = font();
	bottomFont.setPointSize(14);
	bottomFont.setFamily("mikachan");
	bottomFont.setUnderline(true);

	// Convert Ruby
	mConvertRuby = new KawaiiLabel;
	mConvertRuby->setWordWrap(true);
	mConvertRuby->setFont(bottomFont);
	mConvertRuby->setRubyFont(topFont);
	mConvertRuby->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	mConvertRuby->setToolTip( tr("The text that has been converted ends up here.") );

	QGridLayout *gridLayout = new QGridLayout;
	gridLayout->addWidget(entryLabel, 0, 0);
	gridLayout->addWidget(mEntryEdit, 0, 1);
	gridLayout->addWidget(rubyLabel, 1, 0);
	gridLayout->addWidget(mRubyCombo, 1, 1);
	gridLayout->addWidget(resultLabel, 2, 0);
	gridLayout->addWidget(mConvertResult, 2, 1);
	gridLayout->addWidget(mConvertRuby, 3, 1);

	QFrame *line = new QFrame;
	line->setFrameShape(QFrame::HLine);

	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(description);
	layout->addWidget(line);
	layout->addLayout(gridLayout);
	setLayout(layout);

	connect(mEntryEdit, SIGNAL(textChanged()), this, SLOT(updateResult()));
	connect(mConvertResult, SIGNAL(textChanged()), this, SLOT(updateFurigana()));
	connect(mRubyCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(updateResult()));
	connect(mConvertRuby, SIGNAL(textClicked(Qt::MouseButton, int, int)), this, SLOT(handleRubyClick(Qt::MouseButton, int, int)));

	mEntryEdit->setText( QString::fromUtf8("毎日あなたにもらえる勇気") );
};

void FuriganaTab::updateResult()
{
	QString text = chasenRuby(mEntryEdit->toPlainText());

	if( mRubyFormats.contains( mRubyCombo->currentText() ) )
		mConvertResult->setPlainText( parseRuby(text, mRubyFormats.value(mRubyCombo->currentText()) ) );
	else
		mConvertResult->setPlainText(text);
};

void FuriganaTab::updateFurigana()
{
	mConvertRuby->setText( parseRuby(mConvertResult->toPlainText(), Normal) );
};

void FuriganaTab::handleRubyClick(Qt::MouseButton button, int index, int length)
{
	QString text = mConvertRuby->text();

	int openIndex = QRegExp("<ruby>").indexIn(text, index + length);
	int closeIndex = QRegExp("</ruby>").indexIn(text, index + length);

	int isInside = false;
	if( openIndex > closeIndex )
	{
		isInside = true;
	}
	else
	{
		if(openIndex == -1 && closeIndex != -1)
			isInside = true;
		else
			isInside = false;
	}

	QString word;
	if(isInside)
	{
		openIndex = QRegExp("<ruby>").lastIndexIn( text.left(index) );
		word = parseRuby(text.mid(openIndex, (closeIndex + 7) - openIndex), Bottom);
	}
	else
	{
		int currentIndex = 0;
		QList<ChasenEntry> entries = chasenBreakDown(text);
		foreach(ChasenEntry entry, entries)
		{
			// Check if the letter is in this word
			if( index >= currentIndex && index < currentIndex + entry.original.length() )
			{
				word = entry.original;
				break;
			}
			currentIndex += entry.original.length();
		}
	}

	QMessageBox::information(this, tr("Kawaii Demo"), tr("The word you are looking for is: %1").arg(word));
};
