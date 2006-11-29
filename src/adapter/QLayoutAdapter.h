#ifndef THEKLA_QLAYOUTADAPTER_H
#define THEKLA_QLAYOUTADAPTER_H
#include "QObjectAdapter.h"
class QLayoutAdapter : public QObjectAdapter
{
Q_OBJECT
protected:
QLayoutAdapter(QtObject* owner)
 : QObjectAdapter(owner)
{};
public:
virtual ~QLayoutAdapter()
 {}; 
public:
static QtAdapter* create(QtObject* owner)
{
  return new QLayoutAdapter(owner);
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
#endif // THEKLA_QLAYOUTADAPTER_H
