#ifndef THEKLA_QABSTRACTBUTTONADAPTER_H
#define THEKLA_QABSTRACTBUTTONADAPTER_H
#include "QWidgetAdapter.h"
class QAbstractButtonAdapter : public QWidgetAdapter
{
Q_OBJECT
protected:
QAbstractButtonAdapter(QtObject* owner)
 : QWidgetAdapter(owner)
{};
public:
virtual ~QAbstractButtonAdapter()
 {}; 
public:
static QtAdapter* create(QtObject* owner)
{
  return new QAbstractButtonAdapter(owner);
};
public slots:
void pressedSlot
(
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
signalChanged("pressed()", a);
};
void releasedSlot
(
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
signalChanged("released()", a);
};
void clickedSlot
(
bool v0
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
s << QVariant((bool)v0);
signalChanged("clicked(bool)", a);
};
void clickedSlot
(
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
signalChanged("clicked()", a);
};
void toggledSlot
(
bool v0
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
s << QVariant((bool)v0);
signalChanged("toggled(bool)", a);
};
signals:
void pressedSignal
(
)
;
void releasedSignal
(
)
;
void clickedSignal
(
bool v0
)
;
void clickedSignal
(
)
;
void toggledSignal
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

if (signalSignature == "pressed()")
{
emit pressedSignal
(
);
return true;
}
if (signalSignature == "released()")
{
emit releasedSignal
(
);
return true;
}
if (signalSignature == "clicked(bool)")
{
bool v0;
s >> v;
v0 = v.value<bool>();
emit clickedSignal
(
v0
);
return true;
}
if (signalSignature == "clicked()")
{
emit clickedSignal
(
);
return true;
}
if (signalSignature == "toggled(bool)")
{
bool v0;
s >> v;
v0 = v.value<bool>();
emit toggledSignal
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
#endif // THEKLA_QABSTRACTBUTTONADAPTER_H
