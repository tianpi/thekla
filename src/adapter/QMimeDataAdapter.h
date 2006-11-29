#ifndef THEKLA_QMIMEDATAADAPTER_H
#define THEKLA_QMIMEDATAADAPTER_H
#include "QObjectAdapter.h"
class QMimeDataAdapter : public QObjectAdapter
{
Q_OBJECT
protected:
QMimeDataAdapter(QtObject* owner)
 : QObjectAdapter(owner)
{};
public:
virtual ~QMimeDataAdapter()
 {}; 
public:
static QtAdapter* create(QtObject* owner)
{
  return new QMimeDataAdapter(owner);
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


if (QObjectAdapter::signalEmit(signalSignature, paramData))
  return true;

return false;
};
};
#endif // THEKLA_QMIMEDATAADAPTER_H
