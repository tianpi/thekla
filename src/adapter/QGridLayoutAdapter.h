#ifndef THEKLA_QGRIDLAYOUTADAPTER_H
#define THEKLA_QGRIDLAYOUTADAPTER_H
#include "QLayoutAdapter.h"
class QGridLayoutAdapter : public QLayoutAdapter
{
Q_OBJECT
protected:
QGridLayoutAdapter(QtObject* owner)
 : QLayoutAdapter(owner)
{};
public:
virtual ~QGridLayoutAdapter()
 {}; 
public:
static QtAdapter* create(QtObject* owner)
{
  return new QGridLayoutAdapter(owner);
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
#endif // THEKLA_QGRIDLAYOUTADAPTER_H
