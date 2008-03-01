#ifndef __TwinList_h__
#define __TwinList_h__

#include <QtGui/QListWidget>

class TwinScrollBarFilter : public QObject
{
	Q_OBJECT

public:
	TwinScrollBarFilter(QScrollBar *twin = 0, QObject *parent = 0);

protected:
	bool eventFilter(QObject *obj, QEvent *event);

	QScrollBar *mTwin;
};

#include <QtGui/QScrollBar>

class FriendScrollbar : public QScrollBar
{
	friend class TwinScrollBarFilter;
};

class TwinScrollBar;

class TwinList : public QListWidget
{
	Q_OBJECT

public:
	TwinList(QWidget *parent = 0);

	TwinList* twin() const;
	TwinScrollBar* twinScrollBar() const;

public slots:
	void setTwin(TwinList *the_twin);
	void setTwinScrollBar(TwinScrollBar *the_twin);

protected:
	virtual void mouseMoveEvent(QMouseEvent *event);
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseReleaseEvent(QMouseEvent *event);
	virtual void wheelEvent(QWheelEvent *event);

	virtual void keyPressEvent(QKeyEvent *event);
	virtual void keyReleaseEvent(QKeyEvent *event);

	TwinList *mTwin;
};

#endif // __TwinList_h__
