#include "TwinList.h"
#include "TwinScrollBar.h"

#include <QtGui/QMouseEvent>
#include <QtGui/QWheelEvent>
#include <QtGui/QKeyEvent>

TwinScrollBarFilter::TwinScrollBarFilter(QScrollBar *twin, QObject *parent) : QObject(parent)
{
	mTwin = twin;
};

bool TwinScrollBarFilter::eventFilter(QObject *obj, QEvent *event)
{
	if( !event->spontaneous() || !mTwin )
		return QObject::eventFilter(obj, event);

	if(event->type() == QEvent::MouseButtonPress)
	{
		QMouseEvent copy(event->type(), ((QMouseEvent*)event)->pos(), mTwin->mapToGlobal(((QMouseEvent*)event)->pos()),
			((QMouseEvent*)event)->button(), ((QMouseEvent*)event)->buttons(), ((QMouseEvent*)event)->modifiers());
		((FriendScrollbar*)mTwin)->mousePressEvent(&copy);
	}
	else if(event->type() == QEvent::MouseButtonRelease)
	{
		QMouseEvent copy(event->type(), ((QMouseEvent*)event)->pos(), mTwin->mapToGlobal(((QMouseEvent*)event)->pos()),
			((QMouseEvent*)event)->button(), ((QMouseEvent*)event)->buttons(), ((QMouseEvent*)event)->modifiers());
		((FriendScrollbar*)mTwin)->mouseReleaseEvent(&copy);
	}
	else if(event->type() == QEvent::MouseMove)
	{
		QMouseEvent copy(event->type(), ((QMouseEvent*)event)->pos(), mTwin->mapToGlobal(((QMouseEvent*)event)->pos()),
			((QMouseEvent*)event)->button(), ((QMouseEvent*)event)->buttons(), ((QMouseEvent*)event)->modifiers());
		((FriendScrollbar*)mTwin)->mouseMoveEvent(&copy);
	}
	else if(event->type() == QEvent::Wheel)
	{
		QWheelEvent copy(((QWheelEvent*)event)->pos(), mTwin->mapToGlobal(((QWheelEvent*)event)->pos()),
			((QWheelEvent*)event)->delta(), ((QWheelEvent*)event)->buttons(), ((QWheelEvent*)event)->modifiers(), ((QWheelEvent*)event)->orientation());
		((FriendScrollbar*)mTwin)->wheelEvent(&copy);
	}

	return QObject::eventFilter(obj, event);
};

TwinList::TwinList(QWidget *parent) : QListWidget(parent)
{
	//setVerticalScrollBar(new TwinScrollBar(Qt::Vertical, this));
	mTwin = 0;
};

TwinList* TwinList::twin() const
{
	return mTwin;
};

TwinScrollBar* TwinList::twinScrollBar() const
{
	return qobject_cast<TwinScrollBar*>(verticalScrollBar())->twin();
};

void TwinList::setTwin(TwinList *the_twin)
{
	mTwin = the_twin;
	verticalScrollBar()->installEventFilter( new TwinScrollBarFilter(mTwin->verticalScrollBar(), mTwin) );
};

void TwinList::setTwinScrollBar(TwinScrollBar *the_twin)
{
	qobject_cast<TwinScrollBar*>(verticalScrollBar())->setTwin(the_twin);
};

void TwinList::mouseMoveEvent(QMouseEvent *event)
{
	if( event->spontaneous() && mTwin )
	{
		QMouseEvent copy(event->type(), event->pos(), mTwin->mapToGlobal(event->pos()), event->button(), event->buttons(), event->modifiers());
		mTwin->mouseMoveEvent(&copy);
	}

	QListWidget::mouseMoveEvent(event);
};

void TwinList::mousePressEvent(QMouseEvent *event)
{
	if( event->spontaneous() && mTwin )
	{
		QMouseEvent copy(event->type(), event->pos(), mTwin->mapToGlobal(event->pos()), event->button(), event->buttons(), event->modifiers());
		mTwin->mousePressEvent(&copy);
	}

	QListWidget::mousePressEvent(event);
};

void TwinList::mouseReleaseEvent(QMouseEvent *event)
{
	if( event->spontaneous() && mTwin )
	{
		QMouseEvent copy(event->type(), event->pos(), mTwin->mapToGlobal(event->pos()), event->button(), event->buttons(), event->modifiers());
		mTwin->mouseReleaseEvent(&copy);
	}

	QListWidget::mouseReleaseEvent(event);
};

void TwinList::wheelEvent(QWheelEvent *event)
{
	if( event->spontaneous() && mTwin )
	{
		QWheelEvent copy(event->pos(), mTwin->mapToGlobal(event->pos()),
			event->delta(), event->buttons(), event->modifiers(), event->orientation());
		mTwin->wheelEvent(&copy);
	}

	QListWidget::wheelEvent(event);
};

void TwinList::keyPressEvent(QKeyEvent *event)
{
	mTwin->setCurrentRow( currentRow() );
	QListWidget::keyPressEvent(event);
};

void TwinList::keyReleaseEvent(QKeyEvent *event)
{
	mTwin->setCurrentRow( currentRow() );
	QListWidget::keyReleaseEvent(event);
};
