#ifndef __TwinScrollBar_h__
#define __TwinScrollBar_h__

#include <QtGui/QScrollBar>

class TwinScrollBar : public QScrollBar
{
	Q_OBJECT

public:
	TwinScrollBar(QWidget *parent = 0);
	TwinScrollBar(Qt::Orientation orientation, QWidget *parent = 0);

	TwinScrollBar* twin() const;

public slots:
	void setTwin(TwinScrollBar *the_twin);

protected:
	virtual void mouseMoveEvent(QMouseEvent *event);
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseReleaseEvent(QMouseEvent *event);
	virtual void wheelEvent(QWheelEvent *event);

	TwinScrollBar *mTwin;
};

#endif // __TwinScrollBar_h__
