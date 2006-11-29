#ifndef THEKLA_QSTANDARDITEMMODELADAPTER_H
#define THEKLA_QSTANDARDITEMMODELADAPTER_H
#include "QAbstractItemModelAdapter.h"
class QStandardItemModelAdapter : public QAbstractItemModelAdapter
{
Q_OBJECT
protected:
QStandardItemModelAdapter(QtObject* owner)
 : QAbstractItemModelAdapter(owner)
{};
public:
virtual ~QStandardItemModelAdapter()
 {}; 
public:
static QtAdapter* create(QtObject* owner)
{
  return new QStandardItemModelAdapter(owner);
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
#endif // THEKLA_QSTANDARDITEMMODELADAPTER_H
