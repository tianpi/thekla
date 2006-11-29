#ifndef THEKLA_QCHECKBOXADAPTER_H
#define THEKLA_QCHECKBOXADAPTER_H
#include "QAbstractButtonAdapter.h"
class QCheckBoxAdapter : public QAbstractButtonAdapter
{
Q_OBJECT
protected:
QCheckBoxAdapter(QtObject* owner)
 : QAbstractButtonAdapter(owner)
{};
public:
virtual ~QCheckBoxAdapter()
 {}; 
public:
static QtAdapter* create(QtObject* owner)
{
  return new QCheckBoxAdapter(owner);
};
public slots:
void stateChangedSlot
(
int v0
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
s << QVariant((int)v0);
signalChanged("stateChanged(int)", a);
};
signals:
void stateChangedSignal
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

if (signalSignature == "stateChanged(int)")
{
int v0;
s >> v;
v0 = v.value<int>();
emit stateChangedSignal
(
v0
);
return true;
}

if (QAbstractButtonAdapter::signalEmit(signalSignature, paramData))
  return true;

return false;
};
};
#endif // THEKLA_QCHECKBOXADAPTER_H
