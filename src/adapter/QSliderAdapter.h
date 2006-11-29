#ifndef THEKLA_QSLIDERADAPTER_H
#define THEKLA_QSLIDERADAPTER_H
#include "QAbstractSliderAdapter.h"
class QSliderAdapter : public QAbstractSliderAdapter
{
Q_OBJECT
protected:
QSliderAdapter(QtObject* owner)
 : QAbstractSliderAdapter(owner)
{};
public:
virtual ~QSliderAdapter()
 {}; 
public:
static QtAdapter* create(QtObject* owner)
{
  return new QSliderAdapter(owner);
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


if (QAbstractSliderAdapter::signalEmit(signalSignature, paramData))
  return true;

return false;
};
};
#endif // THEKLA_QSLIDERADAPTER_H
