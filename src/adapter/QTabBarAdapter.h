#ifndef THEKLA_QTABBARADAPTER_H
#define THEKLA_QTABBARADAPTER_H
#include "QWidgetAdapter.h"
class QTabBarAdapter : public QWidgetAdapter
{
Q_OBJECT
protected:
QTabBarAdapter(QtObject* owner)
 : QWidgetAdapter(owner)
{};
public:
virtual ~QTabBarAdapter()
 {}; 
public:
static QtAdapter* create(QtObject* owner)
{
  return new QTabBarAdapter(owner);
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
void selectedSlot
(
int v0
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
s << QVariant((int)v0);
signalChanged("selected(int)", a);
};
signals:
void currentChangedSignal
(
int v0
)
;
void selectedSignal
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
if (signalSignature == "selected(int)")
{
int v0;
s >> v;
v0 = v.value<int>();
emit selectedSignal
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
#endif // THEKLA_QTABBARADAPTER_H
