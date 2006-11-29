#ifndef THEKLA_QTABLEWIDGETADAPTER_H
#define THEKLA_QTABLEWIDGETADAPTER_H
#include "QTableViewAdapter.h"
#include <QTableWidgetItem>
class QTableWidgetAdapter : public QTableViewAdapter
{
Q_OBJECT
protected:
QTableWidgetAdapter(QtObject* owner)
 : QTableViewAdapter(owner)
{};
public:
virtual ~QTableWidgetAdapter()
 {}; 
public:
static QtAdapter* create(QtObject* owner)
{
  return new QTableWidgetAdapter(owner);
};
public slots:
void itemSelectionChangedSlot
(
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
signalChanged("itemSelectionChanged()", a);
};
void cellPressedSlot
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
signalChanged("cellPressed(int,int)", a);
};
void cellClickedSlot
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
signalChanged("cellClicked(int,int)", a);
};
void cellDoubleClickedSlot
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
signalChanged("cellDoubleClicked(int,int)", a);
};
void cellActivatedSlot
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
signalChanged("cellActivated(int,int)", a);
};
void cellEnteredSlot
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
signalChanged("cellEntered(int,int)", a);
};
void cellChangedSlot
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
signalChanged("cellChanged(int,int)", a);
};
void currentCellChangedSlot
(
int v0
, 
int v1
, 
int v2
, 
int v3
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
s << QVariant((int)v0);
s << QVariant((int)v1);
s << QVariant((int)v2);
s << QVariant((int)v3);
signalChanged("currentCellChanged(int,int,int,int)", a);
};
signals:
void itemSelectionChangedSignal
(
)
;
void cellPressedSignal
(
int v0
, 
int v1
)
;
void cellClickedSignal
(
int v0
, 
int v1
)
;
void cellDoubleClickedSignal
(
int v0
, 
int v1
)
;
void cellActivatedSignal
(
int v0
, 
int v1
)
;
void cellEnteredSignal
(
int v0
, 
int v1
)
;
void cellChangedSignal
(
int v0
, 
int v1
)
;
void currentCellChangedSignal
(
int v0
, 
int v1
, 
int v2
, 
int v3
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

if (signalSignature == "itemSelectionChanged()")
{
emit itemSelectionChangedSignal
(
);
return true;
}
if (signalSignature == "cellPressed(int,int)")
{
int v0;
s >> v;
v0 = v.value<int>();
int v1;
s >> v;
v1 = v.value<int>();
emit cellPressedSignal
(
v0
, 
v1
);
return true;
}
if (signalSignature == "cellClicked(int,int)")
{
int v0;
s >> v;
v0 = v.value<int>();
int v1;
s >> v;
v1 = v.value<int>();
emit cellClickedSignal
(
v0
, 
v1
);
return true;
}
if (signalSignature == "cellDoubleClicked(int,int)")
{
int v0;
s >> v;
v0 = v.value<int>();
int v1;
s >> v;
v1 = v.value<int>();
emit cellDoubleClickedSignal
(
v0
, 
v1
);
return true;
}
if (signalSignature == "cellActivated(int,int)")
{
int v0;
s >> v;
v0 = v.value<int>();
int v1;
s >> v;
v1 = v.value<int>();
emit cellActivatedSignal
(
v0
, 
v1
);
return true;
}
if (signalSignature == "cellEntered(int,int)")
{
int v0;
s >> v;
v0 = v.value<int>();
int v1;
s >> v;
v1 = v.value<int>();
emit cellEnteredSignal
(
v0
, 
v1
);
return true;
}
if (signalSignature == "cellChanged(int,int)")
{
int v0;
s >> v;
v0 = v.value<int>();
int v1;
s >> v;
v1 = v.value<int>();
emit cellChangedSignal
(
v0
, 
v1
);
return true;
}
if (signalSignature == "currentCellChanged(int,int,int,int)")
{
int v0;
s >> v;
v0 = v.value<int>();
int v1;
s >> v;
v1 = v.value<int>();
int v2;
s >> v;
v2 = v.value<int>();
int v3;
s >> v;
v3 = v.value<int>();
emit currentCellChangedSignal
(
v0
, 
v1
, 
v2
, 
v3
);
return true;
}

if (QTableViewAdapter::signalEmit(signalSignature, paramData))
  return true;

return false;
};
};
#endif // THEKLA_QTABLEWIDGETADAPTER_H
