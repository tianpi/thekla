#ifndef THEKLA_QDIALADAPTER_H
#define THEKLA_QDIALADAPTER_H
#include "QAbstractSliderAdapter.h"
class QDialAdapter : public QAbstractSliderAdapter
{
Q_OBJECT
protected:
QDialAdapter(QtObject* owner)
 : QAbstractSliderAdapter(owner)
{};
public:
virtual ~QDialAdapter()
 {}; 
public:
static QtAdapter* create(QtObject* owner)
{
  return new QDialAdapter(owner);
};
public slots:
void dialPressedSlot
(
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
signalChanged("dialPressed()", a);
};
void dialMovedSlot
(
int v0
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
s << QVariant((int)v0);
signalChanged("dialMoved(int)", a);
};
void dialReleasedSlot
(
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
signalChanged("dialReleased()", a);
};
signals:
void dialPressedSignal
(
)
;
void dialMovedSignal
(
int v0
)
;
void dialReleasedSignal
(
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

if (signalSignature == "dialPressed()")
{
emit dialPressedSignal
(
);
return true;
}
if (signalSignature == "dialMoved(int)")
{
int v0;
s >> v;
v0 = v.value<int>();
emit dialMovedSignal
(
v0
);
return true;
}
if (signalSignature == "dialReleased()")
{
emit dialReleasedSignal
(
);
return true;
}

if (QAbstractSliderAdapter::signalEmit(signalSignature, paramData))
  return true;

return false;
};
};
#endif // THEKLA_QDIALADAPTER_H
