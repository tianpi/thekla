#ifndef THEKLA_QABSTRACTSCROLLAREAADAPTER_H
#define THEKLA_QABSTRACTSCROLLAREAADAPTER_H
#include "QFrameAdapter.h"
class QAbstractScrollAreaAdapter : public QFrameAdapter
{
Q_OBJECT
protected:
QAbstractScrollAreaAdapter(QtObject* owner)
 : QFrameAdapter(owner)
{};
public:
virtual ~QAbstractScrollAreaAdapter()
 {}; 
public:
static QtAdapter* create(QtObject* owner)
{
  return new QAbstractScrollAreaAdapter(owner);
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


if (QFrameAdapter::signalEmit(signalSignature, paramData))
  return true;

return false;
};
};
#endif // THEKLA_QABSTRACTSCROLLAREAADAPTER_H
