#ifndef THEKLA_QABSTRACTITEMVIEWADAPTER_H
#define THEKLA_QABSTRACTITEMVIEWADAPTER_H
#include "QAbstractScrollAreaAdapter.h"
#include <QModelIndex>
class QAbstractItemViewAdapter : public QAbstractScrollAreaAdapter
{
Q_OBJECT
protected:
QAbstractItemViewAdapter(QtObject* owner)
 : QAbstractScrollAreaAdapter(owner)
{};
public:
virtual ~QAbstractItemViewAdapter()
 {}; 
public:
static QtAdapter* create(QtObject* owner)
{
  return new QAbstractItemViewAdapter(owner);
};
public slots:
void viewportEnteredSlot
(
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
signalChanged("viewportEntered()", a);
};
signals:
void viewportEnteredSignal
(
)
;
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

if (signalSignature == "viewportEntered()")
{
emit viewportEnteredSignal
(
);
return true;
}

if (QAbstractScrollAreaAdapter::signalEmit(signalSignature, paramData))
  return true;

return false;
};
};
#endif // THEKLA_QABSTRACTITEMVIEWADAPTER_H
