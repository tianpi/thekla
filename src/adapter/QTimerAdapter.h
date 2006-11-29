#ifndef THEKLA_QTIMERADAPTER_H
#define THEKLA_QTIMERADAPTER_H
#include "QObjectAdapter.h"
class QTimerAdapter : public QObjectAdapter
{
Q_OBJECT
protected:
QTimerAdapter(QtObject* owner)
 : QObjectAdapter(owner)
{};
public:
virtual ~QTimerAdapter()
 {}; 
public:
static QtAdapter* create(QtObject* owner)
{
  return new QTimerAdapter(owner);
};
public slots:
void timeoutSlot
(
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
signalChanged("timeout()", a);
};
signals:
void timeoutSignal
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

if (signalSignature == "timeout()")
{
emit timeoutSignal
(
);
return true;
}

if (QObjectAdapter::signalEmit(signalSignature, paramData))
  return true;

return false;
};
};
#endif // THEKLA_QTIMERADAPTER_H
