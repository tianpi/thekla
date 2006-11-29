#ifndef THEKLA_QSTACKEDLAYOUTADAPTER_H
#define THEKLA_QSTACKEDLAYOUTADAPTER_H
#include "QLayoutAdapter.h"
class QStackedLayoutAdapter : public QLayoutAdapter
{
Q_OBJECT
protected:
QStackedLayoutAdapter(QtObject* owner)
 : QLayoutAdapter(owner)
{};
public:
virtual ~QStackedLayoutAdapter()
 {}; 
public:
static QtAdapter* create(QtObject* owner)
{
  return new QStackedLayoutAdapter(owner);
};
public slots:
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
signals:
void widgetRemovedSignal
(
int v0
)
;
void currentChangedSignal
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

if (QLayoutAdapter::signalEmit(signalSignature, paramData))
  return true;

return false;
};
};
#endif // THEKLA_QSTACKEDLAYOUTADAPTER_H
