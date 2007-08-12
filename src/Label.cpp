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

#include "Label.h"

#include <QtCore/QPair>
#include <QtCore/QStack>
#include <QtCore/QRegExp>
#include <QtCore/QVariant>
#include <QtGui/QPixmap>
#include <QtGui/QPainter>
#include <QtGui/QMouseEvent>
#include <iostream>

class FontChunk
{
public:
	FontChunk() { width = 0; ascent = 0; leftBearing = 0; start = -1, length = 0; };

	int width;
	int ascent;
	int leftBearing;
	int bottomHeight;
	int start, length;
	QPixmap pixmap;
};

class RubyState
{
public:
	~RubyState()
	{
		// Clear out any remaining chunks that should not be there
		foreach(FontChunk *chunk, bottomChunks)
			delete chunk;
		foreach(FontChunk *chunk, topChunks)
			delete chunk;
		foreach(FontChunk *chunk, normalChunks)
			delete chunk;
	};

	QString rubyState;
	QStack<QPen>  topPen;
	QStack<QFont> topFont;
	QStack<QPen>  bottomPen;
	QStack<QFont> bottomFont;

	int lastRubyStart;
	FontChunks bottomChunks, topChunks, normalChunks;
};

KawaiiLabel::KawaiiLabel(QWidget *parent) : QWidget(parent), mRubySpacing(2), mLineSpacing(3), mIgnoreRubyDescent(true),
	mEqualLines(false), mAlignment(Qt::AlignTop | Qt::AlignLeft)
{
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	setMinimumSize(100, 100);
	setMargin(6);

	mTopPen = QPen();
	mTopFont = font();
	mBottomFont = font();
	mTopFont.setPointSize( (int)(mBottomFont.pointSize() * 0.4f + 0.5f) );
	mTopPen.setBrush( palette().brush(QPalette::Active, QPalette::Text) );
	mBottomPen = mTopPen;
};

KawaiiLabel::KawaiiLabel(const QString& text, QWidget *parent) : QWidget(parent), mRubySpacing(2), mLineSpacing(3), mIgnoreRubyDescent(true),
	mEqualLines(false), mAlignment(Qt::AlignTop | Qt::AlignLeft)
{
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	setMinimumSize(100, 100);
	setMargin(6);
	setText(text);

	mTopPen = QPen();
	mTopFont = font();
	mBottomFont = font();
	mTopFont.setPointSize( (int)(mBottomFont.pointSize() * 0.4f + 0.5f) );
	mTopPen.setBrush( palette().brush(QPalette::Active, QPalette::Text) );
	mBottomPen = mTopPen;
};

KawaiiLabel::~KawaiiLabel()
{
	clearChunks();
};

void KawaiiLabel::setText(const QString& text)
{
	mText = text;
	clearChunks();

	RubyState state;
	state.topPen.push(mTopPen);
	state.topFont.push(mTopFont);
	state.bottomPen.push(mBottomPen);
	state.bottomFont.push(mBottomFont);

	int start = 0, end = 0;

	// Loop through and find the index range for each line and process that line
	while( (end = text.indexOf('\n', start)) >= 0 )
	{
		processLine(text, start, end, &state);
		start = end + 1;
	}

	// If there is one more line after the last line break (there usually is, but doesn't have to be), process it too
	if(start < text.count())
		processLine(text, start, text.count(), &state);

	int ascent, lineHeight;
	for(int i = 0; i < mLines.count(); i++)
	{
		ascent = 0;
		foreach(FontChunk *chunk, mLines.at(i))
		{
			if(chunk->ascent > ascent)
				ascent = chunk->ascent;
			if(chunk->ascent > mMaxAscent)
				mMaxAscent = chunk->ascent;
		}
		mLineAscent << ascent;

		lineHeight = 0;
		foreach(FontChunk *chunk, mLines.at(i))
		{
			if(chunk->pixmap.height() > lineHeight)
				lineHeight = chunk->pixmap.height();
			if(chunk->pixmap.height() > mMaxLineHeight)
				mMaxLineHeight = chunk->pixmap.height();
		}
		mLineHeight << lineHeight;
	}

	update();
};

void KawaiiLabel::processLine(const QString& text, int initial_start, int end, RubyState *state)
{
	/// @TODO Do something if there is currently no chunks and we want a line break

	// std::cout << "Processing Line: " << text.toLocal8Bit().data() << std::endl;

	int pos = 0, start = initial_start, spaceIndex;
	bool closing;
	QString tag;

	QRegExp colorRx("color=\"(#[0-9a-fA-F]{6})\"");
	QRegExp faceRx("face=\"([^\"]*)\"");
	QRegExp sizeRx("size=\"([0-9]+)\"");
	QRegExp tagRx("<([^<]+)>");
	tagRx.setMinimal(true);

	while(true)
	{
		// We are looking for tags, if there are no tags, we should break and finish off
		// the rest of the line with the current style settings
		pos = tagRx.indexIn(text, start);
		if(pos < 0)
			break;

		// We are not going to look past the end of our line, so break
		if(pos > end)
			break;

		/*
		std::cout << "Start: " << start << std::endl;
		std::cout << "Matched index: " << pos << std::endl;
		std::cout << "Text length: " << (pos - start) << std::endl;
		std::cout << "Previous text: " << text.mid(start, pos - start).toLocal8Bit().data() << std::endl;
		std::cout << "Matched tag: <" << tagRx.cap(1).toLocal8Bit().data() << ">" << std::endl;
		std::cout << "Matched length: " << tagRx.matchedLength() << std::endl;
		std::cout << "Ruby mode: " << state->rubyState.toLocal8Bit().data() << std::endl;
		*/

		// Before we make any changes to the state, process the text we have so far (if any)
		flushText(text, start, pos, state);

		// If the tag starts with a slash, it's a closing tag
		closing = (tagRx.cap(1).at(0) == '/');

		// std::cout << "Closing tag: " << (closing ? "yes" : "no") << std::endl;
		// std::cout << "==================================================" << std::endl;

		// Strip out slashes from the tag
		tag = tagRx.cap(1);
		tag.remove('/');

		// We want just the tag name, not any of the properties
		if( (spaceIndex = tag.indexOf( QRegExp("\\s") )) != -1 )
			tag = tag.left(spaceIndex);

		// Make it all lowercase, just in case
		tag = tag.toLower();

		/// @TODO Clean this up and make each tag a function or something
		/// @TODO Maybe add bold, italic, and underline to the style stack
		if(tag == "b")
		{
			// Bold
			if(state->rubyState == "rt")
				state->topFont.top().setBold(!closing);
			else
				state->bottomFont.top().setBold(!closing);
		}
		else if(tag == "i")
		{
			// Italics
			if(state->rubyState == "rt")
				state->topFont.top().setItalic(!closing);
			else
				state->bottomFont.top().setItalic(!closing);
		}
		else if(tag == "u")
		{
			// Underline
			if(state->rubyState == "rt")
				state->topFont.top().setUnderline(!closing);
			else
				state->bottomFont.top().setUnderline(!closing);
		}
		else if(tag == "br")
		{
			mLines << state->normalChunks;
			state->normalChunks.clear();
		}
		else if(tag == "font")
		{
			if(closing)
			{
				// Change the style back to the previous one
				state->topPen.pop();
				state->topFont.pop();
				state->bottomPen.pop();
				state->bottomFont.pop();

				// Ensure we always have a style on the stack
				if( !state->topPen.count() )
				{
					state->topPen.push(mTopPen);
					state->topFont.push(mTopFont);
				}

				if( !state->bottomPen.count() )
				{
					state->bottomPen.push(mBottomPen);
					state->bottomFont.push(mBottomFont);
				}
			}
			else
			{
				// Check the 3 possible font properties
				int propertyCount = 0;

				QPen topPen = state->topPen.top();
				QFont topFont = state->topFont.top();
				QPen bottomPen = state->bottomPen.top();
				QFont bottomFont = state->bottomFont.top();

				/// @TODO Allow handeling of more then hex colors
				if(colorRx.indexIn(tagRx.cap(1)) != -1)
				{
					if(state->rubyState != "rt")
						bottomPen.setColor( QColor(colorRx.cap(1)) );
					if(state->rubyState != "rb")
						topPen.setColor( QColor(colorRx.cap(1)) );
	
					propertyCount++;
				}

				if(faceRx.indexIn(tagRx.cap(1)) != -1)
				{
					if(state->rubyState != "rt")
						bottomFont.setFamily( faceRx.cap(1) );
					if(state->rubyState != "rb")
						topFont.setFamily( faceRx.cap(1) );
	
					propertyCount++;
				}

				/// @TODO Handle both pixel and point sizes with a postfix
				if(sizeRx.indexIn(tagRx.cap(1)) != -1)
				{
					if(state->rubyState == "rt")
					{
						topFont.setPointSize( sizeRx.cap(1).toInt() );
					}
					if(state->rubyState == "rb")
					{
						bottomFont.setPointSize( sizeRx.cap(1).toInt() );
					}
					else
					{
						bottomFont.setPointSize( sizeRx.cap(1).toInt() );
						topFont.setPointSize( (int)(state->bottomFont.top().pointSize() * 0.4f + 0.5f) );
					}
	
					propertyCount++;
				}
	
				// If there wasn't any font properties, reset the pen and font
				if(!propertyCount)
				{
					if(state->rubyState != "rt")
					{
						bottomFont = mBottomFont;
						bottomPen = mBottomPen;
					}
					if(state->rubyState != "rb")
					{
						topFont = mTopFont;
						topPen = mTopPen;
					}
				}

				// Push our changed onto the stack
				state->topPen.push(topPen);
				state->topFont.push(topFont);
				state->bottomPen.push(bottomPen);
				state->bottomFont.push(bottomFont);
			}
		}
		else if(tag == "ruby")
		{
			if(closing)
			{
				// std::cout << "Creating ruby width " << state->bottomChunks.count() << " bottom chunks and " <<
					// state->topChunks.count() << " top chunks." << std::endl;

				// create a ruby chunk out of the 2 text buffers
				FontChunk *chunk = createRubyChunk(state->bottomChunks, state->bottomFont.top(), state->topChunks, state->topFont.top());
				chunk->start = state->lastRubyStart;
				chunk->length = state->lastRubyStart - ( start + tagRx.matchedLength() );
				state->normalChunks << chunk;

				// Clear everything out
				state->topChunks.clear();
				state->bottomChunks.clear();
				state->rubyState.clear();
			}
			else
			{
				state->lastRubyStart = start;
				state->rubyState = "ruby";
			}
		}
		else if(tag == "rb")
		{
			if(closing)
				state->rubyState = "ruby";
			else
				state->rubyState = "rb";
		}
		else if(tag == "rt")
		{
			if(closing)
				state->rubyState = "ruby";
			else
				state->rubyState = "rt";
		}
		else
		{
			// No such tag, we will assume this is normal text
		}

		// Calculate our new start
		start = pos + tagRx.matchedLength();
	}

	// If we have any left over text, add it
	flushText(text, start, end, state);

	if( state->normalChunks.count() )
	{
		mLines << state->normalChunks;
		state->normalChunks.clear();
	}

	/*
	QStringList textLines = text.split("\n");
	foreach(QString textLine, textLines)
	{
		QStringList top, bottom;
		{
			QPair<QStringList, QStringList> pair = splitRuby(textLine);
			top = pair.first;
			bottom = pair.second;
		}

		FontChunks chunks;
		for(int i = 0; i < bottom.count(); i++)
		{
			if(top.at(i).isEmpty())
				chunks << createNormalChunks(bottom.at(i), mBottomFont, mPen);
			else
				chunks << createRubyChunk(bottom.at(i), mBottomFont, top.at(i), mTopFont, mPen);
		}

		mLines << chunks;
	}
	*/
};

void KawaiiLabel::flushText(const QString& text, int start, int pos, RubyState *state)
{
	if( state->rubyState.isEmpty() )
	{
		// We are doing normal text
		FontChunks new_chunks;
		new_chunks << createNormalChunks(text.mid(start, pos - start), state->bottomFont.top(), state->bottomPen.top());
		int index = start;
		foreach(FontChunk *chunk, new_chunks)
		{
			chunk->start = start;
			chunk->length = 1;
			start++;
		}
		state->normalChunks << new_chunks;
	}
	else if(state->rubyState == "rb")
	{
		state->bottomChunks << createNormalChunks(text.mid(start, pos - start), state->bottomFont.top(), state->bottomPen.top());
	}
	else if(state->rubyState == "rt")
	{
		state->topChunks << createNormalChunks(text.mid(start, pos - start), state->topFont.top(), state->topPen.top());
	}

	// This is inside a ruby tag but not inside <rb> or <rt>, ignore it
};

void KawaiiLabel::calculateLineSizes(int *docHeight, QList<int> *lineWidths) const
{
	QFontMetrics bottomMetrics(mBottomFont);

	int currentX = leftMargin();
	int leading = bottomMetrics.leading();
	int baseLine = topMargin() - leading - mLineSpacing;

	int lastLineDescent = 0;
	int ascent = 0, lineHeight = 0;

	(*docHeight) = -topMargin();
	lineWidths->clear();

	for(int z = 0; z < mLines.count(); z++)
	{
		FontChunks chunks = mLines.at(z);

		lastLineDescent = lineHeight - ascent - 1;
		ascent = mEqualLines ? mMaxAscent : mLineAscent.at(z);
		lineHeight = mEqualLines ? mMaxLineHeight : mLineHeight.at(z);

		// Handle a new line
		currentX = leftMargin();
		baseLine += lastLineDescent + leading + mLineSpacing + ascent;
		if((baseLine + bottomMargin()) > height())
		{
			lineWidths->append(currentX);
			return;
		}
		(*docHeight) = baseLine + (lineHeight - ascent - 1);

		int runningWidth = 0;

		for(int i = 0; i < chunks.count(); i++)
		{
			runningWidth += chunks.at(i)->width;
			if((runningWidth + rightMargin()) > width())
			{
				runningWidth = 0;
				
				// Handle a new line
				lineWidths->append(currentX);
				currentX = leftMargin();
				baseLine += lastLineDescent + leading + mLineSpacing + ascent;
				if((baseLine + bottomMargin()) > height())
					return;

				(*docHeight) = baseLine + (lineHeight - ascent - 1);
			}

			currentX += chunks.at(i)->width + leading + 1;
		}

		lineWidths->append(currentX);
	}
};

#define carriageReturn() \
	if( mAlignment.testFlag(Qt::AlignHCenter) ) { \
		currentX = (int)((((float)( width() - lineWidths.takeFirst() )) / 2.0f) + 0.5f); } \
	else { \
		int lineWidth = lineWidths.takeFirst(); \
		currentX = mAlignment.testFlag(Qt::AlignLeft) ? leftMargin() : width() - lineWidth - rightMargin(); }

#define lineFeed() \
	baseLine += lastLineDescent + leading + mLineSpacing + ascent; \
	if((baseLine + bottomMargin()) > height()) return

void KawaiiLabel::paintEvent(QPaintEvent *event)
{
	/// @TODO Make everything work with vertical text too

	QFontMetrics bottomMetrics(mBottomFont);
	QPainter painter(this);

	int docHeight = 0;
	QList<int> lineWidths;
	calculateLineSizes(&docHeight, &lineWidths);

	int currentX = leftMargin();
	int leading = bottomMetrics.leading();

	int baseLine = 0;
	if( mAlignment.testFlag(Qt::AlignVCenter) ) // center
	{
		baseLine = (int)((( (float)(height() - docHeight) ) / 2.0f) + 0.5f);
	}
	else // top or bottom
	{
		baseLine = topMargin() - leading - mLineSpacing;
		if( mAlignment.testFlag(Qt::AlignBottom) )
			baseLine += height() - docHeight - bottomMargin();
	}

	int lastLineDescent = 0;
	int ascent = 0, lineHeight = 0;

	for(int z = 0; z < mLines.count(); z++)
	{
		FontChunks chunks = mLines.at(z);

		lastLineDescent = lineHeight - ascent - 1;
		ascent = mEqualLines ? mMaxAscent : mLineAscent.at(z);
		lineHeight = mEqualLines ? mMaxLineHeight : mLineHeight.at(z);

		carriageReturn();
		lineFeed();

		int runningWidth = 0;

		for(int i = 0; i < chunks.count(); i++)
		{
			runningWidth += chunks.at(i)->width;
			if((runningWidth + rightMargin()) > width())
			{
				runningWidth = 0;
				carriageReturn();
				lineFeed();
			}

			painter.drawPixmap(currentX + ((chunks.at(i)->leftBearing < 0) ? chunks.at(i)->leftBearing : 0), baseLine - chunks.at(i)->ascent, chunks.at(i)->pixmap);
			currentX += chunks.at(i)->width + leading + 1;
		}
	}
};

FontChunk* KawaiiLabel::mergeChunks(const FontChunks chunks, const QFont& font) const
{
	if( !chunks.count() )
		return (new FontChunk);

	int width = 0;
	int height = 0;
	int ascent = 0;
	foreach(FontChunk *chunk, chunks)
	{
		width += chunk->width;
		if(chunk->pixmap.height() > height) height = chunk->pixmap.height();
		if(chunk->ascent > ascent) ascent = chunk->ascent;
	}

	int leftBearing = chunks.first()->leftBearing;
	width += ((leftBearing < 0) ? (-leftBearing) : 0);

	int image_width = width + chunks.last()->pixmap.width() - ((leftBearing < 0) ? (-leftBearing) : 0) - chunks.last()->width;

	QPixmap pixmap(image_width, height);
	pixmap.fill(Qt::transparent);

	int currentX = 0;
	int baseLine = ascent;
	int leading = QFontMetrics(font).leading();

	QPainter painter(&pixmap);
	for(int i = 0; i < chunks.count(); i++)
	{
		painter.drawPixmap(currentX + ((chunks.at(i)->leftBearing < 0) ? chunks.at(i)->leftBearing : 0), baseLine - chunks.at(i)->ascent, chunks.at(i)->pixmap);
		currentX += chunks.at(i)->width + leading + 1;
	}

	foreach(FontChunk *chunk, chunks)
		delete chunk;

	FontChunk *chunk = new FontChunk;
	chunk->width = width;
	chunk->ascent = ascent;
	chunk->bottomHeight = height;
	chunk->leftBearing = leftBearing;
	chunk->pixmap = pixmap;

	return chunk;
};

FontChunks KawaiiLabel::createNormalChunks(const QString& string, const QFont& font, const QPen& pen) const
{
	FontChunks list;

	for(int i = 0; i < string.length(); i++)
		list << createChunk(string.at(i), font, pen);

	return list;
};

FontChunk* KawaiiLabel::createRubyChunk(const FontChunks& bottom, const QFont& bottomFont,
	const FontChunks& top, const QFont& topFont) const
{
	FontChunk *topChunk = mergeChunks(top, topFont);
	FontChunk *bottomChunk = mergeChunks(bottom, bottomFont);
	FontChunk *bigger = (topChunk->pixmap.width() > bottomChunk->pixmap.width()) ? topChunk : bottomChunk;

	int image_width = bigger->pixmap.width();
	int height = bottomChunk->pixmap.height() + topChunk->ascent + 1 + mRubySpacing;
	if(!mIgnoreRubyDescent)
		height += (topChunk->pixmap.height() - topChunk->ascent - 1);

	int leftBearing = bigger->leftBearing;
	int width = bigger->width;
	int ascent = height - (bottomChunk->pixmap.height() - bottomChunk->ascent);

	QPixmap pixmap(image_width, height);
	pixmap.fill(Qt::transparent);

	QPainter painter(&pixmap);
	painter.drawPixmap(((image_width - bottomChunk->pixmap.width()) / 2) + ((bottomChunk->leftBearing < 0) ? bottomChunk->leftBearing : 0),
		ascent - bottomChunk->ascent, bottomChunk->pixmap);
	painter.drawPixmap(((image_width - topChunk->pixmap.width()) / 2) + ((topChunk->leftBearing < 0) ? topChunk->leftBearing : 0),
		0, topChunk->pixmap);

	FontChunk *chunk = new FontChunk;
	chunk->width = width;
	chunk->ascent = ascent;
	chunk->bottomHeight = bottomChunk->pixmap.height();
	chunk->leftBearing = leftBearing;
	chunk->pixmap = pixmap;

	delete topChunk;
	delete bottomChunk;

	return chunk;
};

FontChunk* KawaiiLabel::createChunk(const QChar& character, const QFont& font, const QPen& pen) const
{
	QFontMetrics metrics(font);
	int leftBearing = metrics.leftBearing(character);
	int rightBearing = metrics.rightBearing(character);
	int width = metrics.width(character);
	int height = metrics.height();
	int ascent = metrics.ascent();

	int image_width = ((leftBearing < 0) ? (-leftBearing) : 0) + width +
		((rightBearing < 0) ? (-rightBearing) : 0);

	QPixmap pixmap(image_width, height);
	pixmap.fill(Qt::transparent);

	QPainter painter(&pixmap);
	painter.setFont(font);
	painter.setPen(pen);
	painter.drawText((leftBearing < 0) ? (-leftBearing) : 0, ascent, character);

	FontChunk *chunk = new FontChunk;
	chunk->width = width;
	chunk->ascent = ascent;
	chunk->bottomHeight = height;
	chunk->leftBearing = leftBearing;
	chunk->pixmap = pixmap;

	return chunk;
};

QPair<QStringList, QStringList> KawaiiLabel::splitRuby(const QString& text) const
{
	QString ruby;
	QStringList top, bottom;

	int pos = 0;
	int start = 0;

	QRegExp rubyRx("<ruby>(.*)</ruby>");
	rubyRx.setMinimal(true);
	QRegExp bottomRx("<rb>(.*)</rb>");
	bottomRx.setMinimal(true);
	QRegExp topRx("<rt>(.*)</rt>");
	topRx.setMinimal(true);

	while( (pos = rubyRx.indexIn(text, start)) != -1 )
	{
		if( (pos - start) > 0 )
		{
			top << QString();
			bottom << text.mid(start, pos - start);
		}

		ruby = text.mid( pos, rubyRx.matchedLength() );
		start = pos + rubyRx.matchedLength();

		if(bottomRx.indexIn(ruby) != -1)
			bottom << bottomRx.cap(1);
		else
			continue;

		if(topRx.indexIn(ruby) != -1)
			top << topRx.cap(1);
		else
			top << QString();
	}

	if( start < text.count() )
	{
		top << QString();
		bottom << text.mid(start);
	}

	return QPair<QStringList, QStringList>(top, bottom);
};

void KawaiiLabel::clearChunks()
{
	foreach(FontChunks chunks, mLines)
	{
		foreach(FontChunk *chunk, chunks)
			delete chunk;

		chunks.clear();
	}

	mLines.clear();
	mLineAscent.clear();
	mLineHeight.clear();
	mMaxAscent = mMaxLineHeight = 0;
};

int KawaiiLabel::leftMargin() const
{
	return mLeftMargin;
};

void KawaiiLabel::setLeftMargin(int leftMargin)
{
	mLeftMargin = leftMargin;
};

int KawaiiLabel::rightMargin() const
{
	return mRightMargin;
};

void KawaiiLabel::setRightMargin(int rightMargin)
{
	mRightMargin = rightMargin;
};

int KawaiiLabel::bottomMargin() const
{
	return mBottomMargin;
};

void KawaiiLabel::setBottomMargin(int bottomMargin)
{
	mBottomMargin = bottomMargin;
};

int KawaiiLabel::topMargin() const
{
	return mTopMargin;
};

void KawaiiLabel::setTopMargin(int topMargin)
{
	mTopMargin = topMargin;
};

void KawaiiLabel::setMargin(int margin)
{
	setLeftMargin(margin);
	setRightMargin(margin);
	setBottomMargin(margin);
	setTopMargin(margin);
};

QFont KawaiiLabel::font() const
{
	return mBottomFont;
};

QFont KawaiiLabel::rubyFont() const
{
	return mTopFont;
};

void KawaiiLabel::setFont(const QFont& font)
{
	mBottomFont = font;
};

void KawaiiLabel::setRubyFont(const QFont& rubyFont)
{
	mTopFont = rubyFont;
};

QString KawaiiLabel::textAt(const QPoint& point) const
{
	QPair<int, int> index = textIndexAt(point);

	return mText.mid(index.first, index.second);
};

QPair<int, int> KawaiiLabel::textIndexAt(const QPoint& point) const
{
	QFontMetrics bottomMetrics(mBottomFont);

	int docHeight = 0;
	QList<int> lineWidths;
	calculateLineSizes(&docHeight, &lineWidths);

	int currentX = leftMargin();
	int leading = bottomMetrics.leading();

	int baseLine = 0;
	if( mAlignment.testFlag(Qt::AlignVCenter) ) // center
	{
		baseLine = (int)((( (float)(height() - docHeight) ) / 2.0f) + 0.5f);
	}
	else // top or bottom
	{
		baseLine = topMargin() - leading - mLineSpacing;
		if( mAlignment.testFlag(Qt::AlignBottom) )
			baseLine += height() - docHeight - bottomMargin();
	}

	int lastLineDescent = 0;
	int ascent = 0, lineHeight = 0;

	for(int z = 0; z < mLines.count(); z++)
	{
		FontChunks chunks = mLines.at(z);

		lastLineDescent = lineHeight - ascent - 1;
		ascent = mEqualLines ? mMaxAscent : mLineAscent.at(z);
		lineHeight = mEqualLines ? mMaxLineHeight : mLineHeight.at(z);

		carriageReturn();
		lineFeed() QPair<int, int>(0, 0);

		int runningWidth = 0;

		// If we passed over it, return with 0, 0
		if( point.y() < (baseLine - ascent) )
			return QPair<int, int>(0, 0);

		/// @TODO Fix this
		//if( point.y() > (baseLine + (lineHeight - ascent)) )
			//continue;

		int x0, x1, y0, y1;
		for(int i = 0; i < chunks.count(); i++)
		{
			runningWidth += chunks.at(i)->width;
			if((runningWidth + rightMargin()) > width())
			{
				runningWidth = 0;
				carriageReturn();
				lineFeed() QPair<int, int>(0, 0);
			}

			x0 = currentX;
			x1 = x0 + chunks.at(i)->pixmap.width() + ((chunks.at(i)->leftBearing < 0) ? chunks.at(i)->leftBearing : 0);
			y0 = baseLine - chunks.at(i)->ascent;
			y1 = y0 + chunks.at(i)->pixmap.height();

			/*
			std::cout << "x: " << point.x() << std::endl;
			std::cout << "y: " << point.y() << std::endl;
			std::cout << "x0: " << x0 << std::endl;
			std::cout << "y0: " << y0 << std::endl;
			std::cout << "x1: " << x1 << std::endl;
			std::cout << "y1: " << y1 << std::endl;
			*/

			if(point.x() >= x0 && point.x() <= x1)
			{
				if(point.y() >= y0 && point.y() <= y1)
				{
					if(chunks.at(i)->pixmap.height() == chunks.at(i)->bottomHeight)
						return QPair<int, int>( chunks.at(i)->start, chunks.at(i)->length );

					QString type;
					if( point.y() > (y0 + chunks.at(i)->pixmap.height() - chunks.at(i)->bottomHeight) )
						type = "rb";
					else
						type = "rt";

					int start = mText.indexOf("<" + type + ">", chunks.at(i)->start) + 4;
					int end = mText.indexOf("</" + type + ">", chunks.at(i)->start);

					return QPair<int, int>(start, end - start);

					return QPair<int, int>( chunks.at(i)->start, chunks.at(i)->length );
				}
			}

			currentX += chunks.at(i)->width + leading + 1;
		}
	}

	return QPair<int, int>(0, 0);
};

Qt::Alignment KawaiiLabel::alignment() const
{
	return mAlignment;
};

void KawaiiLabel::setAlignment(Qt::Alignment alignment)
{
	mAlignment = alignment;
};

void KawaiiLabel::mousePressEvent(QMouseEvent *event)
{
	std::cout << "Text at (" << event->x() << ", " << event->y() << "): " << textAt( event->pos() ).toLocal8Bit().data() << std::endl;
};
