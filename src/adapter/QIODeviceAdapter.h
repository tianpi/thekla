#ifndef THEKLA_QIODEVICEADAPTER_H
#define THEKLA_QIODEVICEADAPTER_H
#include "QObjectAdapter.h"
class QIODeviceAdapter : public QObjectAdapter
{
Q_OBJECT
protected:
QIODeviceAdapter(QtObject* owner)
 : QObjectAdapter(owner)
{};
public:
virtual ~QIODeviceAdapter()
 {}; 
public:
static QtAdapter* create(QtObject* owner)
{
  return new QIODeviceAdapter(owner);
};
public slots:
void readyReadSlot
(
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
signalChanged("readyRead()", a);
};
void aboutToCloseSlot
(
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
signalChanged("aboutToClose()", a);
};
signals:
void readyReadSignal
(
)
;
void aboutToCloseSignal
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

if (signalSignature == "readyRead()")
{
emit readyReadSignal
(
);
return true;
}
if (signalSignature == "aboutToClose()")
{
emit aboutToCloseSignal
(
);
return true;
}

if (QObjectAdapter::signalEmit(signalSignature, paramData))
  return true;

return false;
};
};
#endif // THEKLA_QIODEVICEADAPTER_H
