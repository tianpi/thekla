#ifndef THEKLA_QDIRMODELADAPTER_H
#define THEKLA_QDIRMODELADAPTER_H
#include "QAbstractItemModelAdapter.h"
class QDirModelAdapter : public QAbstractItemModelAdapter
{
Q_OBJECT
protected:
QDirModelAdapter(QtObject* owner)
 : QAbstractItemModelAdapter(owner)
{};
public:
virtual ~QDirModelAdapter()
 {}; 
public:
static QtAdapter* create(QtObject* owner)
{
  return new QDirModelAdapter(owner);
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
#endif // THEKLA_QDIRMODELADAPTER_H
