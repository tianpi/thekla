#ifndef THEKLA_QLABELADAPTER_H
#define THEKLA_QLABELADAPTER_H
#include "QFrameAdapter.h"
class QLabelAdapter : public QFrameAdapter
{
Q_OBJECT
protected:
QLabelAdapter(QtObject* owner)
 : QFrameAdapter(owner)
{};
public:
virtual ~QLabelAdapter()
 {}; 
public:
static QtAdapter* create(QtObject* owner)
{
  return new QLabelAdapter(owner);
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
#endif // THEKLA_QLABELADAPTER_H
