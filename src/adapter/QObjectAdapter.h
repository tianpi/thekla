#ifndef THEKLA_QOBJECTADAPTER_H
#define THEKLA_QOBJECTADAPTER_H
#include "QtAdapter.h"
#include <QObject>
class QObjectAdapter : public QtAdapter
{
Q_OBJECT
protected:
QObjectAdapter(QtObject* owner)
 : QtAdapter(owner)
{};
public:
virtual ~QObjectAdapter()
 {}; 
public:
static QtAdapter* create(QtObject* owner)
{
  return new QObjectAdapter(owner);
};
public slots:
void destroyedSlot
(
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
signalChanged("destroyed()", a);
};
signals:
void destroyedSignal
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

if (signalSignature == "destroyed()")
{
emit destroyedSignal
(
);
return true;
}

if (QtAdapter::signalEmit(signalSignature, paramData))
  return true;

return false;
};
};
#endif // THEKLA_QOBJECTADAPTER_H
