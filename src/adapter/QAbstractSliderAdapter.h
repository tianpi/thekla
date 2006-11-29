#ifndef THEKLA_QABSTRACTSLIDERADAPTER_H
#define THEKLA_QABSTRACTSLIDERADAPTER_H
#include "QWidgetAdapter.h"
class QAbstractSliderAdapter : public QWidgetAdapter
{
Q_OBJECT
protected:
QAbstractSliderAdapter(QtObject* owner)
 : QWidgetAdapter(owner)
{};
public:
virtual ~QAbstractSliderAdapter()
 {}; 
public:
static QtAdapter* create(QtObject* owner)
{
  return new QAbstractSliderAdapter(owner);
};
public slots:
void valueChangedSlot
(
int v0
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
s << QVariant((int)v0);
signalChanged("valueChanged(int)", a);
};
void sliderPressedSlot
(
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
signalChanged("sliderPressed()", a);
};
void sliderMovedSlot
(
int v0
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
s << QVariant((int)v0);
signalChanged("sliderMoved(int)", a);
};
void sliderReleasedSlot
(
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
signalChanged("sliderReleased()", a);
};
void rangeChangedSlot
(
int v0
, 
int v1
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
s << QVariant((int)v0);
s << QVariant((int)v1);
signalChanged("rangeChanged(int,int)", a);
};
void actionTriggeredSlot
(
int v0
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
s << QVariant((int)v0);
signalChanged("actionTriggered(int)", a);
};
signals:
void valueChangedSignal
(
int v0
)
;
void sliderPressedSignal
(
)
;
void sliderMovedSignal
(
int v0
)
;
void sliderReleasedSignal
(
)
;
void rangeChangedSignal
(
int v0
, 
int v1
)
;
void actionTriggeredSignal
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

if (signalSignature == "valueChanged(int)")
{
int v0;
s >> v;
v0 = v.value<int>();
emit valueChangedSignal
(
v0
);
return true;
}
if (signalSignature == "sliderPressed()")
{
emit sliderPressedSignal
(
);
return true;
}
if (signalSignature == "sliderMoved(int)")
{
int v0;
s >> v;
v0 = v.value<int>();
emit sliderMovedSignal
(
v0
);
return true;
}
if (signalSignature == "sliderReleased()")
{
emit sliderReleasedSignal
(
);
return true;
}
if (signalSignature == "rangeChanged(int,int)")
{
int v0;
s >> v;
v0 = v.value<int>();
int v1;
s >> v;
v1 = v.value<int>();
emit rangeChangedSignal
(
v0
, 
v1
);
return true;
}
if (signalSignature == "actionTriggered(int)")
{
int v0;
s >> v;
v0 = v.value<int>();
emit actionTriggeredSignal
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
#endif // THEKLA_QABSTRACTSLIDERADAPTER_H
