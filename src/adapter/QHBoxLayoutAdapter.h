#ifndef THEKLA_QHBOXLAYOUTADAPTER_H
#define THEKLA_QHBOXLAYOUTADAPTER_H
#include "QBoxLayoutAdapter.h"
class QHBoxLayoutAdapter : public QBoxLayoutAdapter
{
Q_OBJECT
protected:
QHBoxLayoutAdapter(QtObject* owner)
 : QBoxLayoutAdapter(owner)
{};
public:
virtual ~QHBoxLayoutAdapter()
 {}; 
public:
static QtAdapter* create(QtObject* owner)
{
  return new QHBoxLayoutAdapter(owner);
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


if (QBoxLayoutAdapter::signalEmit(signalSignature, paramData))
  return true;

return false;
};
};
#endif // THEKLA_QHBOXLAYOUTADAPTER_H
