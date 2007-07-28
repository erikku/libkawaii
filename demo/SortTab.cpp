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

#include "SortTab.h"

#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QListWidget>
#include <QtGui/QListWidgetItem>

#include <kawaii/ChasenUtils.h>
#include <kawaii/KanaConversion.h>
#include <kawaii/LineEdit.h>

SortTab::SortTab(QWidget *parent) : QWidget(parent)
{
	QLabel *description = new QLabel;
	description->setWordWrap(true);
	description->setAlignment(Qt::AlignLeft | Qt::AlignTop);
	description->setText( tr("<p>This demo shows how we can expand upon the conversion and furigana demos and use the kana"
		" representation of the words to create an all kana key that can be used to sort a list in a,i,u,e,o order. You can"
		" add words by typing them in the field bellow and pressing enter. If you type in a entry with ruby, it will use"
		" that for a key instead of kana. To remove a entry, double click it.</p>") );

	QLabel *entryLabel = new QLabel( tr("Text:") );
	// mEntryEdit = new QLineEdit;
	mEntryEdit = new KawaiiLineEdit;
	mEntryEdit->setToolTip( tr("Enter the text you want to sort in this field.") );

	QPushButton *romajiModeButton = new QPushButton( tr("Romaji Mode") );
	romajiModeButton->setCheckable(true);
	romajiModeButton->setChecked( ((KawaiiLineEdit*)mEntryEdit)->romajiMode() );
	connect(romajiModeButton, SIGNAL(toggled(bool)), mEntryEdit, SLOT(setRomajiMode(bool)));

	QHBoxLayout *entryLayout = new QHBoxLayout;
	entryLayout->addWidget(entryLabel);
	entryLayout->addWidget(mEntryEdit);
	entryLayout->addWidget(romajiModeButton);

	// Sort List Widgets
	QLabel *normalLabel = new QLabel( tr("<b>Normal</b>") );
	normalLabel->setAlignment(Qt::AlignCenter);
	mNormalSort = new QListWidget;
	mNormalSort->setToolTip( tr("This list shows the entries sorted with the normal sort function.") );
	QLabel *kanaLabel = new QLabel( tr("<b>Kana Key</b>") );
	kanaLabel->setAlignment(Qt::AlignCenter);
	mChasenSort = new QListWidget;
	mChasenSort->setToolTip( tr("This list shows the entries sorted by a kana key.") );

	// Sort grid layout
	QGridLayout *chasenGrid = new QGridLayout;
	chasenGrid->addWidget(normalLabel, 0, 0);
	chasenGrid->addWidget(mNormalSort, 1, 0);
	chasenGrid->addWidget(kanaLabel, 0, 1);
	chasenGrid->addWidget(mChasenSort, 1, 1);

	QFrame *line = new QFrame;
	line->setFrameShape(QFrame::HLine);

	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(description);
	layout->addWidget(line);
	layout->addLayout(entryLayout);
	layout->addLayout(chasenGrid);
	setLayout(layout);

	connect(mEntryEdit, SIGNAL(returnPressed()), this, SLOT(addSortItem()));
	connect(mNormalSort, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(removeSortItem(QListWidgetItem*)));
	connect(mChasenSort, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(removeSortItem(QListWidgetItem*)));

	mEntryEdit->setText("test"); addSortItem();
	mEntryEdit->setText( QString::fromUtf8("美しい") ); addSortItem();
	mEntryEdit->setText( QString::fromUtf8("まだ") ); addSortItem();
	mEntryEdit->setText( QString::fromUtf8("未来") ); addSortItem();
	mEntryEdit->setText( QString::fromUtf8("モーニング娘。") ); addSortItem();
	mEntryEdit->setText( QString::fromUtf8("毎日") ); addSortItem();
	mEntryEdit->setText("example"); addSortItem();
};

void SortTab::addSortItem()
{
	QString entry = mEntryEdit->text();
	if(entry.isEmpty())
		return;

	mEntryEdit->clear();

	QString kana;
	if( containsRuby(entry) )
	{
		kana = parseRuby(entry, Top);
		entry = parseRuby(entry, Bottom);
	}
	else
	{
		kana = chasenKana(entry, false);
	}

	mEntries[kana] = entry;
	refreshLists();
};

void SortTab::removeSortItem(QListWidgetItem *item)
{
	mEntries.remove( mEntries.key(item->text()) );
	refreshLists();
};

void SortTab::refreshLists()
{
	QStringList keys = mEntries.keys();
	QStringList values = mEntries.values();
	qSort(keys); qSort(values);

	mNormalSort->clear();
	mChasenSort->clear();

	foreach(QString item, values)
		mNormalSort->addItem(item);
	foreach(QString key, keys)
		mChasenSort->addItem( mEntries.value(key) );
};
