#ifndef THEKLA_QSCROLLAREAADAPTER_H
#define THEKLA_QSCROLLAREAADAPTER_H
#include "QAbstractScrollAreaAdapter.h"
class QScrollAreaAdapter : public QAbstractScrollAreaAdapter
{
Q_OBJECT
protected:
QScrollAreaAdapter(QtObject* owner)
 : QAbstractScrollAreaAdapter(owner)
{};
public:
virtual ~QScrollAreaAdapter()
 {}; 
public:
static QtAdapter* create(QtObject* owner)
{
  return new QScrollAreaAdapter(owner);
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


if (QAbstractScrollAreaAdapter::signalEmit(signalSignature, paramData))
  return true;

return false;
};
};
#endif // THEKLA_QSCROLLAREAADAPTER_H
