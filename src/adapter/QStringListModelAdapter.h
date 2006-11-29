#ifndef THEKLA_QSTRINGLISTMODELADAPTER_H
#define THEKLA_QSTRINGLISTMODELADAPTER_H
#include "QAbstractListModelAdapter.h"
class QStringListModelAdapter : public QAbstractListModelAdapter
{
Q_OBJECT
protected:
QStringListModelAdapter(QtObject* owner)
 : QAbstractListModelAdapter(owner)
{};
public:
virtual ~QStringListModelAdapter()
 {}; 
public:
static QtAdapter* create(QtObject* owner)
{
  return new QStringListModelAdapter(owner);
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


if (QAbstractListModelAdapter::signalEmit(signalSignature, paramData))
  return true;

return false;
};
};
#endif // THEKLA_QSTRINGLISTMODELADAPTER_H
