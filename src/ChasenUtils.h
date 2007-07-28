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

#ifndef __ChasenUtils_h__
#define __ChasenUtils_h__

#include <QtCore/QList>
#include <QtCore/QString>

/*
 * Original text
 * Katakana representation
 * Base form
 * Part of Speech (- Sub-part of Speech)
 * Type of conjugation (・ Sub-conjugation type)(Adjective, Verb, Special, etc.)
 * Conjugation Form
 */

class ChasenEntry
{
public:
	QString original;
	QString kana;
	QString baseForm;
	QString partOfSpeech;
	QString conjugationType;
	QString conjugationForm;
};

QList<ChasenEntry> chasenBreakDown(const QString& text);

typedef enum ConversionType
{
	None = 0,
	Kana,
	Romaji,
	Ruby
};

QString chasenConvert(const QString& text, ConversionType type = None, bool withSpaces = false);
QString chasenKana(const QString& text, bool withSpaces = true);
QString chasenRomaji(const QString& text, bool withSpaces = true);
QString chasenRuby(const QString& text, bool withSpaces = false);

#endif // __ChasenUtils_h__
