#ifndef THEKLA_QABSTRACTSPINBOXADAPTER_H
#define THEKLA_QABSTRACTSPINBOXADAPTER_H
#include "QWidgetAdapter.h"
class QAbstractSpinBoxAdapter : public QWidgetAdapter
{
Q_OBJECT
protected:
QAbstractSpinBoxAdapter(QtObject* owner)
 : QWidgetAdapter(owner)
{};
public:
virtual ~QAbstractSpinBoxAdapter()
 {}; 
public:
static QtAdapter* create(QtObject* owner)
{
  return new QAbstractSpinBoxAdapter(owner);
};
public slots:
void editingFinishedSlot
(
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
signalChanged("editingFinished()", a);
};
signals:
void editingFinishedSignal
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

if (signalSignature == "editingFinished()")
{
emit editingFinishedSignal
(
);
return true;
}

if (QWidgetAdapter::signalEmit(signalSignature, paramData))
  return true;

return false;
};
};
#endif // THEKLA_QABSTRACTSPINBOXADAPTER_H
