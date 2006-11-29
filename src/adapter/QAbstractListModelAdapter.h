#ifndef THEKLA_QABSTRACTLISTMODELADAPTER_H
#define THEKLA_QABSTRACTLISTMODELADAPTER_H
#include "QAbstractItemModelAdapter.h"
class QAbstractListModelAdapter : public QAbstractItemModelAdapter
{
Q_OBJECT
protected:
QAbstractListModelAdapter(QtObject* owner)
 : QAbstractItemModelAdapter(owner)
{};
public:
virtual ~QAbstractListModelAdapter()
 {}; 
public:
static QtAdapter* create(QtObject* owner)
{
  return new QAbstractListModelAdapter(owner);
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
#endif // THEKLA_QABSTRACTLISTMODELADAPTER_H
