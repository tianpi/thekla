#ifndef THEKLA_QVBOXLAYOUTADAPTER_H
#define THEKLA_QVBOXLAYOUTADAPTER_H
#include "QBoxLayoutAdapter.h"
class QVBoxLayoutAdapter : public QBoxLayoutAdapter
{
Q_OBJECT
protected:
QVBoxLayoutAdapter(QtObject* owner)
 : QBoxLayoutAdapter(owner)
{};
public:
virtual ~QVBoxLayoutAdapter()
 {}; 
public:
static QtAdapter* create(QtObject* owner)
{
  return new QVBoxLayoutAdapter(owner);
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
#endif // THEKLA_QVBOXLAYOUTADAPTER_H
