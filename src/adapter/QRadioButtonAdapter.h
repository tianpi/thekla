#ifndef THEKLA_QRADIOBUTTONADAPTER_H
#define THEKLA_QRADIOBUTTONADAPTER_H
#include "QAbstractButtonAdapter.h"
class QRadioButtonAdapter : public QAbstractButtonAdapter
{
Q_OBJECT
protected:
QRadioButtonAdapter(QtObject* owner)
 : QAbstractButtonAdapter(owner)
{};
public:
virtual ~QRadioButtonAdapter()
 {}; 
public:
static QtAdapter* create(QtObject* owner)
{
  return new QRadioButtonAdapter(owner);
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
#endif // THEKLA_QRADIOBUTTONADAPTER_H
