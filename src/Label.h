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

#include <QtCore/QStringList>
#include <QtCore/QList>

#include <QtGui/QWidget>
#include <QtGui/QPen>

class FontChunk;
typedef QList<FontChunk*> FontChunks;

class RubyState;

/// @TODO Implement minimum size and size hints
/// @TODO Get KawaiiLabel handeling style sheets
/// @TODO Implement justify line alignment

class KawaiiLabel : public QWidget
{
	Q_OBJECT

	Q_PROPERTY(int leftMargin READ leftMargin WRITE setLeftMargin)
	Q_PROPERTY(int rightMargin READ rightMargin WRITE setRightMargin)
	Q_PROPERTY(int bottomMargin READ bottomMargin WRITE setBottomMargin)
	Q_PROPERTY(int topMargin READ topMargin WRITE setTopMargin)
	Q_PROPERTY(bool wordWrap READ wordWrap WRITE setWordWrap)
	Q_PROPERTY(bool equalLines READ equalLines WRITE setEqualLines)
	Q_PROPERTY(int lineSpacing READ lineSpacing WRITE setLineSpacing)
	Q_PROPERTY(bool ignoreRubyDescent READ ignoreRubyDescent WRITE setIgnoreRubyDescent)
	Q_PROPERTY(Qt::Alignment alignment READ alignment WRITE setAlignment)
	Q_PROPERTY(QFont font READ font WRITE setFont)
	Q_PROPERTY(QFont rubyFont READ rubyFont WRITE setRubyFont)
	Q_PROPERTY(QPen pen READ pen WRITE setPen)
	Q_PROPERTY(QPen rubyPen READ rubyPen WRITE setRubyPen)

public:
	KawaiiLabel(QWidget *parent = 0);
	KawaiiLabel(const QString& text, QWidget *parent = 0);
	~KawaiiLabel();

	int leftMargin() const;
	int rightMargin() const;
	int bottomMargin() const;
	int topMargin() const;
	QString text() const;

	bool wordWrap() const;
	bool equalLines() const;
	int lineSpacing() const;
	bool ignoreRubyDescent() const;
	Qt::Alignment alignment() const;
	QFont rubyFont() const;

	QPen pen() const;
	QPen rubyPen() const;

	QString textAt(const QPoint& point) const;
	QPair<int, int> textIndexAt(const QPoint& point) const;

signals:
	void textClicked(Qt::MouseButton button, const QString& text);
	void textClicked(Qt::MouseButton button, int index, int length);

public slots:
	void setMargin(int margin);
	void setLeftMargin(int leftMargin);
	void setRightMargin(int rightMargin);
	void setBottomMargin(int bottomMargin);
	void setTopMargin(int topMargin);
	void setText(const QString& text);

	void setWordWrap(bool wrap);
	void setEqualLines(bool equal);
	void setLineSpacing(int spacing);
	void setIgnoreRubyDescent(bool ignore);
	void setAlignment(Qt::Alignment alignment);
	void setRubyFont(const QFont& rubyFont);

	void setPen(const QPen& pen);
	void setRubyPen(const QPen& pen);

protected:
	virtual void changeEvent(QEvent *event);
	virtual void mouseReleaseEvent(QMouseEvent *event);

	void clearChunks();
	void generateChunks();

	FontChunk* mergeChunks(const FontChunks chunks, const QFont& font) const;
	FontChunk* createChunk(const QChar& character, const QFont& font, const QPen& pen) const;
	FontChunks createNormalChunks(const QString& string, const QFont& font, const QPen& pen) const;
	FontChunk* createRubyChunk(const FontChunks& bottom, const QFont& bottomFont,
		const FontChunks& top, const QFont& topFont) const;

	void processLine(const QString& text, int initial_start, int end, RubyState *state);
	void flushText(const QString& text, int start, int pos, RubyState *state);
	void calculateLineSizes(int *docHeight, QList<int> *lineWidths) const;

	QPair<QStringList, QStringList> splitRuby(const QString& text) const;
	virtual void paintEvent(QPaintEvent *event);

	QString mText;
	QList<FontChunks> mLines;

	int mMaxAscent, mMaxLineHeight;
	QList<int> mLineAscent, mLineHeight;

	Qt::Alignment mAlignment;

	QFont mRubyFont;
	QPen mBottomPen, mTopPen;

	bool mWordWrap;
	int mRubySpacing;
	int mLineSpacing;
	bool mEqualLines;
	bool mIgnoreRubyDescent;

	int mTopMargin, mBottomMargin, mLeftMargin, mRightMargin;
};
