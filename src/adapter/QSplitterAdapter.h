#ifndef THEKLA_QSPLITTERADAPTER_H
#define THEKLA_QSPLITTERADAPTER_H
#include "QFrameAdapter.h"
class QSplitterAdapter : public QFrameAdapter
{
Q_OBJECT
protected:
QSplitterAdapter(QtObject* owner)
 : QFrameAdapter(owner)
{};
public:
virtual ~QSplitterAdapter()
 {}; 
public:
static QtAdapter* create(QtObject* owner)
{
  return new QSplitterAdapter(owner);
};
public slots:
void splitterMovedSlot
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
signalChanged("splitterMoved(int,int)", a);
};
signals:
void splitterMovedSignal
(
int v0
, 
int v1
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

if (signalSignature == "splitterMoved(int,int)")
{
int v0;
s >> v;
v0 = v.value<int>();
int v1;
s >> v;
v1 = v.value<int>();
emit splitterMovedSignal
(
v0
, 
v1
);
return true;
}

if (QFrameAdapter::signalEmit(signalSignature, paramData))
  return true;

return false;
};
};
#endif // THEKLA_QSPLITTERADAPTER_H
