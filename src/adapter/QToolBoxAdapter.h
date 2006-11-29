#ifndef THEKLA_QTOOLBOXADAPTER_H
#define THEKLA_QTOOLBOXADAPTER_H
#include "QFrameAdapter.h"
class QToolBoxAdapter : public QFrameAdapter
{
Q_OBJECT
protected:
QToolBoxAdapter(QtObject* owner)
 : QFrameAdapter(owner)
{};
public:
virtual ~QToolBoxAdapter()
 {}; 
public:
static QtAdapter* create(QtObject* owner)
{
  return new QToolBoxAdapter(owner);
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
signals:
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

if (QFrameAdapter::signalEmit(signalSignature, paramData))
  return true;

return false;
};
};
#endif // THEKLA_QTOOLBOXADAPTER_H
