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

#include "ConversionTab.h"

#include <QtGui/QLabel>
#include <QtGui/QTextEdit>
#include <QtGui/QComboBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>

#include <kawaii/ChasenUtils.h>

ConversionTab::ConversionTab(QWidget *parent) : QWidget(parent)
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
	description->setText( tr("<p>This demo converts Japanese between various formats. To space Japanese properly, we need"
		" to know how to split Japanese into words and/or components. Since Chasen anylizes Japanese and gives us detailed"
		" information about each word and/or scentence component, it is ideal for this process.") );

	// Text Edit
	QLabel *entryLabel = new QLabel( tr("Text:") );
	entryLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);
	mEntryEdit = new QTextEdit;
	mEntryEdit->setAcceptRichText(false);
	mEntryEdit->setToolTip( tr("Enter the text you want to convert in this field.") );

	// Type Combo
	QLabel *typeLabel = new QLabel( tr("Type:") );
	QLabel *rubyLabel = new QLabel( tr("Ruby:") );
	mTypeCombo = new QComboBox;
	mTypeCombo->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
	mTypeCombo->setToolTip( tr("Select how to convert the text here.") );
	mTypeCombo->addItems( QStringList() << tr("Leave as is") << tr("Kana")  << tr("Kana with Spaces")
		<< tr("Romaji") << tr("Romaji with Spaces") << tr("Ruby") << tr("Ruby with Spaces") );
	mRubyCombo = new QComboBox;
	mRubyCombo->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
	mRubyCombo->addItem( tr("Leave as is") );
	mRubyCombo->addItems( mRubyFormats.keys() );
	mRubyCombo->setToolTip( tr("Select how to convert ruby tags here.") );

	// Type Layout
	QHBoxLayout *typeLayout = new QHBoxLayout;
	typeLayout->setMargin(0);
	typeLayout->addWidget(mTypeCombo);
	typeLayout->addWidget(rubyLabel);
	typeLayout->addWidget(mRubyCombo);

	// Convert Result
	QLabel *resultLabel = new QLabel( tr("Result:") );
	resultLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);
	mConvertResult = new QTextEdit;
	mConvertResult->setAcceptRichText(false);
	mConvertResult->setReadOnly(true);
	mConvertResult->setToolTip( tr("The text that has been converted ends up here.") );

	QGridLayout *gridLayout = new QGridLayout;
	gridLayout->addWidget(entryLabel, 0, 0);
	gridLayout->addWidget(mEntryEdit, 0, 1);
	gridLayout->addWidget(typeLabel, 1, 0);
	gridLayout->addLayout(typeLayout, 1, 1);
	gridLayout->addWidget(resultLabel, 2, 0);
	gridLayout->addWidget(mConvertResult, 2, 1);

	QFrame *line = new QFrame;
	line->setFrameShape(QFrame::HLine);

	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(description);
	layout->addWidget(line);
	layout->addLayout(gridLayout);
	setLayout(layout);

	connect(mEntryEdit, SIGNAL(textChanged()), this, SLOT(updateConversion()));
	connect(mTypeCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(updateConversion()));
	connect(mRubyCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(updateConversion()));

	mEntryEdit->setText( QString::fromUtf8("私は日本人です。名前はミユキです。") );
};

void ConversionTab::updateConversion()
{
	QString text = mEntryEdit->toPlainText();
	text = romajiToKana(text);

	switch( mTypeCombo->currentIndex() )
	{
		case 1:
			text = chasenKana(text, false);
			break;
		case 2:
			text = chasenKana(text, true);
			break;
		case 3:
			text = chasenRomaji(text, false);
			break;
		case 4:
			text = chasenRomaji(text, true);
			break;
		case 5:
			text = chasenRuby(text, false);
			break;
		case 6:
			text = chasenRuby(text, true);
			break;
	};

	if( mRubyFormats.contains( mRubyCombo->currentText() ) )
		text = parseRuby( text, mRubyFormats.value( mRubyCombo->currentText() ) );

	mConvertResult->setPlainText(text);
};
