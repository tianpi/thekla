#ifndef THEKLA_QEVENTLOOPADAPTER_H
#define THEKLA_QEVENTLOOPADAPTER_H
#include "QObjectAdapter.h"
class QEventLoopAdapter : public QObjectAdapter
{
Q_OBJECT
protected:
QEventLoopAdapter(QtObject* owner)
 : QObjectAdapter(owner)
{};
public:
virtual ~QEventLoopAdapter()
 {}; 
public:
static QtAdapter* create(QtObject* owner)
{
  return new QEventLoopAdapter(owner);
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
#endif // THEKLA_QEVENTLOOPADAPTER_H
