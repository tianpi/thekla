#ifndef THEKLA_QPROXYMODELADAPTER_H
#define THEKLA_QPROXYMODELADAPTER_H
#include "QAbstractItemModelAdapter.h"
class QProxyModelAdapter : public QAbstractItemModelAdapter
{
Q_OBJECT
protected:
QProxyModelAdapter(QtObject* owner)
 : QAbstractItemModelAdapter(owner)
{};
public:
virtual ~QProxyModelAdapter()
 {}; 
public:
static QtAdapter* create(QtObject* owner)
{
  return new QProxyModelAdapter(owner);
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
#endif // THEKLA_QPROXYMODELADAPTER_H
