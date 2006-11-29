#ifndef THEKLA_QOBJECTCLEANUPHANDLERADAPTER_H
#define THEKLA_QOBJECTCLEANUPHANDLERADAPTER_H
#include "QObjectAdapter.h"
class QObjectCleanupHandlerAdapter : public QObjectAdapter
{
Q_OBJECT
protected:
QObjectCleanupHandlerAdapter(QtObject* owner)
 : QObjectAdapter(owner)
{};
public:
virtual ~QObjectCleanupHandlerAdapter()
 {}; 
public:
static QtAdapter* create(QtObject* owner)
{
  return new QObjectCleanupHandlerAdapter(owner);
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
#endif // THEKLA_QOBJECTCLEANUPHANDLERADAPTER_H
