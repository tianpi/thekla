#ifndef THEKLA_QABSTRACTPROXYMODELADAPTER_H
#define THEKLA_QABSTRACTPROXYMODELADAPTER_H
#include "QAbstractItemModelAdapter.h"
class QAbstractProxyModelAdapter : public QAbstractItemModelAdapter
{
Q_OBJECT
protected:
QAbstractProxyModelAdapter(QtObject* owner)
 : QAbstractItemModelAdapter(owner)
{};
public:
virtual ~QAbstractProxyModelAdapter()
 {}; 
public:
static QtAdapter* create(QtObject* owner)
{
  return new QAbstractProxyModelAdapter(owner);
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


if (QAbstractItemModelAdapter::signalEmit(signalSignature, paramData))
  return true;

return false;
};
};
#endif // THEKLA_QABSTRACTPROXYMODELADAPTER_H
