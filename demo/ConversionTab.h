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

#ifndef __ConversionTab_h__
#define __ConversionTab_h__

#include <QtCore/QMap>
#include <QtGui/QWidget>

#include <kawaii/KanaConversion.h>

class QTextEdit;
class QComboBox;

class ConversionTab : public QWidget
{
	Q_OBJECT

public:
	ConversionTab(QWidget *parent = 0);

public slots:
	void updateConversion();

protected:
	QMap<QString, RubyFormat> mRubyFormats;

	QTextEdit *mEntryEdit;
	QTextEdit *mConvertResult;

	QComboBox *mTypeCombo;
	QComboBox *mRubyCombo;
};

#endif // __ConversionTab_h__
