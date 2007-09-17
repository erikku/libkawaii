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

#include "ChasenUtils.h"
#include "configure.h"

#include <QtGui/QApplication>
#include <QtCore/QStringList>
#include <QtCore/QTextCodec>
#include <QtCore/QStack>
#include <QtCore/QMap>
#include <QtCore/QDir>
#include <chasen.h>
#include <iostream>
#include "KanaConversion.h"

bool chasenLoaded = false;

QMap<QString, QString> punctuation;
QMap<QString, QString> brackets;
QMap<QString, QString> bracketCounterparts;

void loadChasen()
{
	if(chasenLoaded)
		return;

	char *argv[5];

	QByteArray app = qApp->arguments().at(0).toLocal8Bit();

#if defined(Q_OS_WIN32)
	QByteArray path = QDir::toNativeSeparators(qApp->applicationDirPath() + "/chasenrc").toLocal8Bit();
#else
	QByteArray path = QDir::toNativeSeparators(QString(LIBKAWAII_INSTALL_PREFIX) + "/share/libkawaii/chasenrc").toLocal8Bit();
#endif

	char *argv0 = app.data();
	char *argv1 = "-r";
	char *argv2 = path.data();
	char *argv3 = "-i";
	char *argv4 = "w";

	argv[0] = argv0;
	argv[1] = argv1;
	argv[2] = argv2;
	argv[3] = argv3;
	argv[4] = argv4;

	punctuation[ QString::fromUtf8("。") ] = QString::fromUtf8(".");
	punctuation[ QString::fromUtf8("？") ] = QString::fromUtf8("?");
	punctuation[ QString::fromUtf8("！") ] = QString::fromUtf8("!");
	punctuation[ QString::fromUtf8("、") ] = QString::fromUtf8(",");
	punctuation[ QString::fromUtf8("…") ] = QString::fromUtf8("...");

	punctuation[ QString::fromUtf8(".") ] = QString::fromUtf8(".");
	punctuation[ QString::fromUtf8("?") ] = QString::fromUtf8("?");
	punctuation[ QString::fromUtf8("!") ] = QString::fromUtf8("!");
	punctuation[ QString::fromUtf8(",") ] = QString::fromUtf8(",");
	punctuation[ QString::fromUtf8("...") ] = QString::fromUtf8("...");
	
	brackets[ QString::fromUtf8("「") ] = QString::fromUtf8("\"");
	brackets[ QString::fromUtf8("」") ] = QString::fromUtf8("\"");
	brackets[ QString::fromUtf8("『") ] = QString::fromUtf8("[ ");
	brackets[ QString::fromUtf8("』") ] = QString::fromUtf8("]");
	brackets[ QString::fromUtf8("【") ] = QString::fromUtf8("[");
	brackets[ QString::fromUtf8("】") ] = QString::fromUtf8("]");
	brackets[ QString::fromUtf8("《") ] = QString::fromUtf8("<");
	brackets[ QString::fromUtf8("》") ] = QString::fromUtf8(">");

	brackets[ QString::fromUtf8("(") ] = QString::fromUtf8("(");
	brackets[ QString::fromUtf8(")") ] = QString::fromUtf8(")");
	brackets[ QString::fromUtf8("\"") ] = QString::fromUtf8("\"");
	brackets[ QString::fromUtf8("]") ] = QString::fromUtf8("]");
	brackets[ QString::fromUtf8("[") ] = QString::fromUtf8("[");
	brackets[ QString::fromUtf8(">") ] = QString::fromUtf8(">");
	brackets[ QString::fromUtf8("<") ] = QString::fromUtf8("<");

	bracketCounterparts[ QString::fromUtf8("\"") ] = QString::fromUtf8("\"");
	bracketCounterparts[ QString::fromUtf8("'") ] = QString::fromUtf8("'");
	bracketCounterparts[ QString::fromUtf8("「") ] = QString::fromUtf8("」");
	bracketCounterparts[ QString::fromUtf8("」") ] = QString::fromUtf8("「");
	bracketCounterparts[ QString::fromUtf8("『") ] = QString::fromUtf8("』");
	bracketCounterparts[ QString::fromUtf8("』") ] = QString::fromUtf8("『");
	bracketCounterparts[ QString::fromUtf8("【") ] = QString::fromUtf8("】");
	bracketCounterparts[ QString::fromUtf8("】") ] = QString::fromUtf8("【");
	bracketCounterparts[ QString::fromUtf8("《") ] = QString::fromUtf8("》");
	bracketCounterparts[ QString::fromUtf8("》") ] = QString::fromUtf8("《");
	bracketCounterparts[ QString::fromUtf8("(") ] = QString::fromUtf8(")");
	bracketCounterparts[ QString::fromUtf8(")") ] = QString::fromUtf8("(");
	bracketCounterparts[ QString::fromUtf8("<") ] = QString::fromUtf8(">");
	bracketCounterparts[ QString::fromUtf8(">") ] = QString::fromUtf8("<");
	bracketCounterparts[ QString::fromUtf8("[") ] = QString::fromUtf8("]");
	bracketCounterparts[ QString::fromUtf8("]") ] = QString::fromUtf8("[");

	std::cerr << argv[0] << " " << argv[1] << " " << argv[2] << " " << argv[3] << " " << argv[4] << std::endl;

	chasen_getopt_argv(argv, 0);
	chasenLoaded = true;
};

QList<ChasenEntry> chasenBreakDown(const QString& text)
{
	loadChasen();

	QStringList lines = QString::fromUtf8( chasen_sparse_tostr( text.toUtf8().data() ) ).split("\n");

	while(lines.last() != "EOS")
	{
		if(!lines.count())
			break;

		lines.removeLast();
	}

	QList<ChasenEntry> list;

	foreach(QString line, lines)
	{
		QStringList fields = line.split("\t");

		if(fields.count() != 6)
			return list;

		ChasenEntry entry;
		entry.original = fields.at(0);
		entry.kana = fields.at(1);
		entry.baseForm = fields.at(2);
		entry.partOfSpeech = fields.at(3);
		entry.conjugationType = fields.at(4);
		entry.conjugationForm = fields.at(5);
		list << entry;
	}

	return list;
};

QString chasenChunkConvert(const QString& chunk, ConversionType type, bool withSpaces)
{
	QList<ChasenEntry> entries = chasenBreakDown(chunk);

	QString final;
	QString prevType;
	QString workingText;
	QString finalBracket;
	QStack<QString> lastBracket;
	lastBracket.push("nill");

	for(int i = 0; i < entries.count(); i++)
	{
		if( prevType == "kanji" )
		{
			if( !punctuation.keys().contains( entries.at(i).original ) && !brackets.keys().contains( entries.at(i).original ) )
				final += " ";
		}
		else if( prevType == "punctuation" )
		{
			if( !brackets.keys().contains( entries.at(i).original ) )
				final += " ";
		}

		// These are symbols or numbers
		if( entries.at(i).partOfSpeech.left(2) == QString::fromUtf8("記号")
			|| entries.at(i).partOfSpeech.left(4) == QString::fromUtf8("名詞-数") )
		{
			if( brackets.keys().contains( entries.at(i).original ) )
			{
				finalBracket = (type == Romaji) ? brackets.value(entries.at(i).original) : entries.at(i).original;

				if( finalBracket == lastBracket.top() )
				{
					lastBracket.pop();
					final += finalBracket + " ";
				}
				else
				{
					if(prevType == "kanji" || prevType == "punctuation")
						final += " ";

					lastBracket.push(finalBracket);
					final += finalBracket;
				}

				prevType = "bracket";
			}
			else if( punctuation.keys().contains( entries.at(i).original ) )
			{
				final += (type == Romaji) ? punctuation.value(entries.at(i).original) : entries.at(i).original;
				prevType = "punctuation";
			}
			else
			{
				final += entries.at(i).original;
				prevType = "symbol";
			}
		}
		else // This is Japanese text (called kanji here)
		{
			// This is a auxiliary verb or dependent verb
			if( entries.at(i).partOfSpeech.left(3) == QString::fromUtf8("助動詞") || entries.at(i).partOfSpeech.left(6) == QString::fromUtf8("動詞-非自立") )
			{
				if(i > 0)
				{
					if( entries.at(i - 1).partOfSpeech.left(2) == QString::fromUtf8("動詞") ) // The previous entry was a verb
						final.resize( final.size() - 1 );
				}
			}

			// We can assume these are katakana words since the original and the katakana version always
			// match and the only other things it could be are symbols which are handled above.
			if( entries.at(i).original == entries.at(i).kana )
			{
				final += entries.at(i).original;
			}
			else // These are not katakana, so let's convert them to hiragana (if requested)
			{
				if(type == Kana || type == Romaji)
				{
					workingText = entries.at(i).kana.isEmpty() ? entries.at(i).original : katakanaToHiragana(entries.at(i).kana);

					if( type == Romaji )
					{
						if( entries.at(i).partOfSpeech.left(2) == QString::fromUtf8("助詞") )
						{
							if( workingText == QString::fromUtf8("は") )
								workingText = QString::fromUtf8("わ");
							else if( workingText == QString::fromUtf8("を") )
								workingText = QString::fromUtf8("お");
							else if( workingText == QString::fromUtf8("へ") )
								workingText = QString::fromUtf8("え");
						}
					}

					final += workingText;
				}
				else if(type == Ruby)
				{
					workingText = katakanaToHiragana(entries.at(i).kana);

					if(entries.at(i).original == workingText) // This word doesn't have furigana
						final += entries.at(i).original;
					else
						final += reduceRuby(entries.at(i).original, workingText);
				}
				else // No conversion
				{
					final += entries.at(i).original;
				}
			}

			prevType = "kanji";
		}
	}

	// Strip spaces
	if(!withSpaces)
		final.replace(" ", QString());

	// Convert romaji
	if(type == Romaji)
		final = kanaToRomaji(final, true);

	return final.trimmed();
}

QString chasenConvert(const QString& text, ConversionType type, bool withSpaces)
{
	loadChasen();

	int start = 0, index;
	QString final, current;
	QRegExp spaceMatcher("(\\s+)");
	while( (index = spaceMatcher.indexIn(text, start)) != -1 )
	{
		current = text.mid( start, index - start );
		if(!current.isEmpty())
			current = chasenChunkConvert(current, type, withSpaces);

		final += current + spaceMatcher.cap(1);
		start = index + spaceMatcher.matchedLength();
	}

	current = text.mid(start);
	if( !current.isEmpty() )
		final += chasenChunkConvert(current, type, withSpaces);

	return final;
};

QString chasenKana(const QString& text, bool withSpaces)
{
	return chasenConvert(text, Kana, withSpaces);
};

QString chasenRomaji(const QString& text, bool withSpaces)
{
	return chasenConvert(text, Romaji, withSpaces);
};

QString chasenRuby(const QString& text, bool withSpaces)
{
	return chasenConvert(text, Ruby, withSpaces);
};
