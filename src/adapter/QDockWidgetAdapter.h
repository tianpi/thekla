#ifndef THEKLA_QDOCKWIDGETADAPTER_H
#define THEKLA_QDOCKWIDGETADAPTER_H
#include "QWidgetAdapter.h"
#include <QDockWidget>
#include <Qt>
class QDockWidgetAdapter : public QWidgetAdapter
{
Q_OBJECT
protected:
QDockWidgetAdapter(QtObject* owner)
 : QWidgetAdapter(owner)
{};
public:
virtual ~QDockWidgetAdapter()
 {}; 
public:
static QtAdapter* create(QtObject* owner)
{
  return new QDockWidgetAdapter(owner);
};
public slots:
void topLevelChangedSlot
(
bool v0
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
s << QVariant((bool)v0);
signalChanged("topLevelChanged(bool)", a);
};
signals:
void topLevelChangedSignal
(
bool v0
)
;
public:
virtual bool signalEmit
(
const QString & signalSignature
,
const QByteArray & paramData
)
{

QByteArray a(paramData);
QDataStream s(&a, QIODevice::ReadOnly);
QVariant v;

if (signalSignature == "topLevelChanged(bool)")
{
bool v0;
s >> v;
v0 = v.value<bool>();
emit topLevelChangedSignal
(
v0
);
return true;
}

if (QWidgetAdapter::signalEmit(signalSignature, paramData))
  return true;

return false;
};
};
#endif // THEKLA_QDOCKWIDGETADAPTER_H
