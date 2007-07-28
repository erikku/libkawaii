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

#include "BasicTab.h"

#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QTableWidget>

#include <kawaii/ChasenUtils.h>

BasicTab::BasicTab(QWidget *parent) : QWidget(parent)
{
	QLabel *description = new QLabel;
	description->setWordWrap(true);
	description->setAlignment(Qt::AlignLeft | Qt::AlignTop);
	description->setText( tr("<p>This demo shows the output that chasen produces when given a string. Chasen will take each"
		" scentence component and output it on it's own line. It will end with a line that reads <i>EOS</i> to indicate"
		" the end of the list. As Chasen is a Japanese library, all information is returned in Japanese. Each line consists"
		" of 6 sections, separated by tabs. Any number of sections could be blank. The sections are as follows:</p>"
		"<ol><li>Original text</li>"
		"<li>Katakana representation</li>"
		"<li>Base form</li>"
		"<li>Part of Speech (- Sub-part of Speech)</li>"
		"<li>Type of conjugation (middle-dot Sub-conjugation type) (Adjective, Verb, Special, etc.)</li>"
		"<li>Conjugation Form</li></ol>") );

	QLabel *entryLabel = new QLabel( tr("Text:") );
	QLineEdit *entryEdit = new QLineEdit;
	entryEdit->setToolTip( tr("Enter the text you want to pass to Chasen in this field.") );

	QHBoxLayout *entryLayout = new QHBoxLayout;
	entryLayout->addWidget(entryLabel);
	entryLayout->addWidget(entryEdit);

	mChasenTable = new QTableWidget;
	mChasenTable->setRowCount(0);
	mChasenTable->setColumnCount(6);
	mChasenTable->setHorizontalHeaderLabels( QStringList() << tr("Original Text") << tr("Phonetic Version") << tr("Base Form")
		<< tr("Part of Speech") << tr("Conjugation Type") << tr("Conjugation Form") );
	mChasenTable->verticalHeader()->hide();

	QFrame *line = new QFrame;
	line->setFrameShape(QFrame::HLine);

	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(description);
	layout->addWidget(line);
	layout->addLayout(entryLayout);
	layout->addWidget(mChasenTable);
	setLayout(layout);

	connect(entryEdit, SIGNAL(textChanged(const QString&)), this, SLOT(updateTable(const QString&)));
	entryEdit->setText( QString::fromUtf8("初めまして。僕はエリックだ。") );
};

void BasicTab::updateTable(const QString& text)
{
	QList<ChasenEntry> entries = chasenBreakDown(text);
	mChasenTable->setRowCount( entries.count() );

	for(int i = 0; i < entries.count(); i++)
	{
		mChasenTable->setItem( i, 0, new QTableWidgetItem(entries.at(i).original) );
		mChasenTable->setItem( i, 1, new QTableWidgetItem(entries.at(i).kana) );
		mChasenTable->setItem( i, 2, new QTableWidgetItem(entries.at(i).baseForm) );
		mChasenTable->setItem( i, 3, new QTableWidgetItem(entries.at(i).partOfSpeech) );
		mChasenTable->setItem( i, 4, new QTableWidgetItem(entries.at(i).conjugationType) );
		mChasenTable->setItem( i, 5, new QTableWidgetItem(entries.at(i).conjugationForm) );
	}

	mChasenTable->resizeColumnsToContents();
};
