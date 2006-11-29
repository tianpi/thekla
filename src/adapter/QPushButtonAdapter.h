#ifndef THEKLA_QPUSHBUTTONADAPTER_H
#define THEKLA_QPUSHBUTTONADAPTER_H
#include "QAbstractButtonAdapter.h"
class QPushButtonAdapter : public QAbstractButtonAdapter
{
Q_OBJECT
protected:
QPushButtonAdapter(QtObject* owner)
 : QAbstractButtonAdapter(owner)
{};
public:
virtual ~QPushButtonAdapter()
 {}; 
public:
static QtAdapter* create(QtObject* owner)
{
  return new QPushButtonAdapter(owner);
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


if (QAbstractButtonAdapter::signalEmit(signalSignature, paramData))
  return true;

return false;
};
};
#endif // THEKLA_QPUSHBUTTONADAPTER_H
