#ifndef THEKLA_QFILEADAPTER_H
#define THEKLA_QFILEADAPTER_H
#include "QIODeviceAdapter.h"
class QFileAdapter : public QIODeviceAdapter
{
Q_OBJECT
protected:
QFileAdapter(QtObject* owner)
 : QIODeviceAdapter(owner)
{};
public:
virtual ~QFileAdapter()
 {}; 
public:
static QtAdapter* create(QtObject* owner)
{
  return new QFileAdapter(owner);
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
#endif // THEKLA_QFILEADAPTER_H
