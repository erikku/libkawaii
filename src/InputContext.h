#ifndef __InputContext_h__
#define __InputContext_h__

#include <QtCore/QHash>
#include <QtGui/QInputContext>
#include <QtGui/QInputContextPlugin>

class QDialog;
class QWidget;

class InputInstance
{
public:
	QString preeditString;
};

class KawaiiInputContextPlugin : public QInputContextPlugin
{
public:
	virtual QInputContext* create(const QString& key);
	virtual QString description(const QString& key);
	virtual QString displayName(const QString& key);
	virtual QStringList keys() const;
	virtual QStringList languages(const QString& key);
};

class KawaiiInputContext : public QInputContext
{
	Q_OBJECT

public:
	KawaiiInputContext(QObject *parent = 0);
	virtual ~KawaiiInputContext();

	virtual bool filterEvent(const QEvent *event);
	virtual QString identifierName();
	virtual QString language();
	virtual void reset();
	virtual bool isComposing() const;

private:
	bool mCtrlDown;
	bool mComposing;

	QWidget *imSelection;
	QWidget *currentWidget;

	QHash<QWidget*, InputInstance> mInstances;
};

#endif // __InputContext_h__
