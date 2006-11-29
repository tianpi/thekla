#ifndef THEKLA_QSORTFILTERPROXYMODELADAPTER_H
#define THEKLA_QSORTFILTERPROXYMODELADAPTER_H
#include "QAbstractProxyModelAdapter.h"
class QSortFilterProxyModelAdapter : public QAbstractProxyModelAdapter
{
Q_OBJECT
protected:
QSortFilterProxyModelAdapter(QtObject* owner)
 : QAbstractProxyModelAdapter(owner)
{};
public:
virtual ~QSortFilterProxyModelAdapter()
 {}; 
public:
static QtAdapter* create(QtObject* owner)
{
  return new QSortFilterProxyModelAdapter(owner);
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


if (QAbstractProxyModelAdapter::signalEmit(signalSignature, paramData))
  return true;

return false;
};
};
#endif // THEKLA_QSORTFILTERPROXYMODELADAPTER_H
