#ifndef THEKLA_QBOXLAYOUTADAPTER_H
#define THEKLA_QBOXLAYOUTADAPTER_H
#include "QLayoutAdapter.h"
class QBoxLayoutAdapter : public QLayoutAdapter
{
Q_OBJECT
protected:
QBoxLayoutAdapter(QtObject* owner)
 : QLayoutAdapter(owner)
{};
public:
virtual ~QBoxLayoutAdapter()
 {}; 
public:
static QtAdapter* create(QtObject* owner)
{
  return new QBoxLayoutAdapter(owner);
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


if (QLayoutAdapter::signalEmit(signalSignature, paramData))
  return true;

return false;
};
};
#endif // THEKLA_QBOXLAYOUTADAPTER_H
