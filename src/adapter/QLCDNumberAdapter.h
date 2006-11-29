#ifndef THEKLA_QLCDNUMBERADAPTER_H
#define THEKLA_QLCDNUMBERADAPTER_H
#include "QFrameAdapter.h"
class QLCDNumberAdapter : public QFrameAdapter
{
Q_OBJECT
protected:
QLCDNumberAdapter(QtObject* owner)
 : QFrameAdapter(owner)
{};
public:
virtual ~QLCDNumberAdapter()
 {}; 
public:
static QtAdapter* create(QtObject* owner)
{
  return new QLCDNumberAdapter(owner);
};
public slots:
void overflowSlot
(
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
signalChanged("overflow()", a);
};
signals:
void overflowSignal
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

if (signalSignature == "overflow()")
{
emit overflowSignal
(
);
return true;
}

if (QFrameAdapter::signalEmit(signalSignature, paramData))
  return true;

return false;
};
};
#endif // THEKLA_QLCDNUMBERADAPTER_H
