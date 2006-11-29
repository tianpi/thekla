#ifndef THEKLA_QSCROLLBARADAPTER_H
#define THEKLA_QSCROLLBARADAPTER_H
#include "QAbstractSliderAdapter.h"
class QScrollBarAdapter : public QAbstractSliderAdapter
{
Q_OBJECT
protected:
QScrollBarAdapter(QtObject* owner)
 : QAbstractSliderAdapter(owner)
{};
public:
virtual ~QScrollBarAdapter()
 {}; 
public:
static QtAdapter* create(QtObject* owner)
{
  return new QScrollBarAdapter(owner);
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
#endif // THEKLA_QSCROLLBARADAPTER_H
