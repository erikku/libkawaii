#include "TwinScrollBar.h"

#include <QtGui/QMouseEvent>
#include <QtGui/QWheelEvent>

TwinScrollBar::TwinScrollBar(QWidget *parent) : QScrollBar(parent)
{
	mTwin = 0;
};

TwinScrollBar::TwinScrollBar(Qt::Orientation orientation, QWidget *parent) : QScrollBar(orientation, parent)
{
	mTwin = 0;
};

TwinScrollBar* TwinScrollBar::twin() const
{
	return mTwin;
};

void TwinScrollBar::setTwin(TwinScrollBar *the_twin)
{
	mTwin = the_twin;
};

void TwinScrollBar::mouseMoveEvent(QMouseEvent *event)
{
	if( event->spontaneous() && mTwin )
	{
		QMouseEvent copy(event->type(), event->pos(), mTwin->mapToGlobal(event->pos()), event->button(), event->buttons(), event->modifiers());
		mTwin->mouseMoveEvent(&copy);
	}

	QScrollBar::mouseMoveEvent(event);
};

void TwinScrollBar::mousePressEvent(QMouseEvent *event)
{
	if( event->spontaneous() && mTwin )
	{
		QMouseEvent copy(event->type(), event->pos(), mTwin->mapToGlobal(event->pos()), event->button(), event->buttons(), event->modifiers());
		mTwin->mousePressEvent(&copy);
	}

	QScrollBar::mousePressEvent(event);
};

void TwinScrollBar::mouseReleaseEvent(QMouseEvent *event)
{
	if( event->spontaneous() && mTwin )
	{
		QMouseEvent copy(event->type(), event->pos(), mTwin->mapToGlobal(event->pos()), event->button(), event->buttons(), event->modifiers());
		mTwin->mouseReleaseEvent(&copy);
	}

	QScrollBar::mouseReleaseEvent(event);
};

void TwinScrollBar::wheelEvent(QWheelEvent *event)
{
	if( event->spontaneous() && mTwin )
	{
		QWheelEvent copy(event->pos(), mTwin->mapToGlobal(event->pos()),
			event->delta(), event->buttons(), event->modifiers(), event->orientation());
		mTwin->wheelEvent(&copy);
	}

	QScrollBar::wheelEvent(event);
};
