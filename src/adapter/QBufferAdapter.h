#ifndef THEKLA_QBUFFERADAPTER_H
#define THEKLA_QBUFFERADAPTER_H
#include "QIODeviceAdapter.h"
class QBufferAdapter : public QIODeviceAdapter
{
Q_OBJECT
protected:
QBufferAdapter(QtObject* owner)
 : QIODeviceAdapter(owner)
{};
public:
virtual ~QBufferAdapter()
 {}; 
public:
static QtAdapter* create(QtObject* owner)
{
  return new QBufferAdapter(owner);
};
public slots:
signals:
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


if (QIODeviceAdapter::signalEmit(signalSignature, paramData))
  return true;

return false;
};
};
#endif // THEKLA_QBUFFERADAPTER_H
