#ifndef THEKLA_QITEMDELEGATEADAPTER_H
#define THEKLA_QITEMDELEGATEADAPTER_H
#include "QAbstractItemDelegateAdapter.h"
class QItemDelegateAdapter : public QAbstractItemDelegateAdapter
{
Q_OBJECT
protected:
QItemDelegateAdapter(QtObject* owner)
 : QAbstractItemDelegateAdapter(owner)
{};
public:
virtual ~QItemDelegateAdapter()
 {}; 
public:
static QtAdapter* create(QtObject* owner)
{
  return new QItemDelegateAdapter(owner);
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


if (QAbstractItemDelegateAdapter::signalEmit(signalSignature, paramData))
  return true;

return false;
};
};
#endif // THEKLA_QITEMDELEGATEADAPTER_H
