#ifndef THEKLA_QSTACKEDWIDGETADAPTER_H
#define THEKLA_QSTACKEDWIDGETADAPTER_H
#include "QFrameAdapter.h"
class QStackedWidgetAdapter : public QFrameAdapter
{
Q_OBJECT
protected:
QStackedWidgetAdapter(QtObject* owner)
 : QFrameAdapter(owner)
{};
public:
virtual ~QStackedWidgetAdapter()
 {}; 
public:
static QtAdapter* create(QtObject* owner)
{
  return new QStackedWidgetAdapter(owner);
};
public slots:
void currentChangedSlot
(
int v0
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
s << QVariant((int)v0);
signalChanged("currentChanged(int)", a);
};
void widgetRemovedSlot
(
int v0
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
s << QVariant((int)v0);
signalChanged("widgetRemoved(int)", a);
};
signals:
void currentChangedSignal
(
int v0
)
;
void widgetRemovedSignal
(
int v0
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

if (signalSignature == "currentChanged(int)")
{
int v0;
s >> v;
v0 = v.value<int>();
emit currentChangedSignal
(
v0
);
return true;
}
if (signalSignature == "widgetRemoved(int)")
{
int v0;
s >> v;
v0 = v.value<int>();
emit widgetRemovedSignal
(
v0
);
return true;
}

if (QFrameAdapter::signalEmit(signalSignature, paramData))
  return true;

return false;
};
};
#endif // THEKLA_QSTACKEDWIDGETADAPTER_H
