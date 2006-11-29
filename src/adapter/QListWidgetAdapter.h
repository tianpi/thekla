#ifndef THEKLA_QLISTWIDGETADAPTER_H
#define THEKLA_QLISTWIDGETADAPTER_H
#include "QListViewAdapter.h"
#include <QListWidgetItem>
#include <QString>
class QListWidgetAdapter : public QListViewAdapter
{
Q_OBJECT
protected:
QListWidgetAdapter(QtObject* owner)
 : QListViewAdapter(owner)
{};
public:
virtual ~QListWidgetAdapter()
 {}; 
public:
static QtAdapter* create(QtObject* owner)
{
  return new QListWidgetAdapter(owner);
};
public slots:
void currentTextChangedSlot
(
QString v0
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
s << QVariant((QString)v0);
signalChanged("currentTextChanged(QString)", a);
};
void currentRowChangedSlot
(
int v0
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
s << QVariant((int)v0);
signalChanged("currentRowChanged(int)", a);
};
void itemSelectionChangedSlot
(
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
signalChanged("itemSelectionChanged()", a);
};
signals:
void currentTextChangedSignal
(
QString v0
)
;
void currentRowChangedSignal
(
int v0
)
;
void itemSelectionChangedSignal
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

if (signalSignature == "currentTextChanged(QString)")
{
QString v0;
s >> v;
v0 = v.value<QString>();
emit currentTextChangedSignal
(
v0
);
return true;
}
if (signalSignature == "currentRowChanged(int)")
{
int v0;
s >> v;
v0 = v.value<int>();
emit currentRowChangedSignal
(
v0
);
return true;
}
if (signalSignature == "itemSelectionChanged()")
{
emit itemSelectionChangedSignal
(
);
return true;
}

if (QListViewAdapter::signalEmit(signalSignature, paramData))
  return true;

return false;
};
};
#endif // THEKLA_QLISTWIDGETADAPTER_H
