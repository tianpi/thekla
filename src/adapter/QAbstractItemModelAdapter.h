#ifndef THEKLA_QABSTRACTITEMMODELADAPTER_H
#define THEKLA_QABSTRACTITEMMODELADAPTER_H
#include "QObjectAdapter.h"
#include <QModelIndex>
#include <Qt>
class QAbstractItemModelAdapter : public QObjectAdapter
{
Q_OBJECT
protected:
QAbstractItemModelAdapter(QtObject* owner)
 : QObjectAdapter(owner)
{};
public:
virtual ~QAbstractItemModelAdapter()
 {}; 
public:
static QtAdapter* create(QtObject* owner)
{
  return new QAbstractItemModelAdapter(owner);
};
public slots:
void layoutChangedSlot
(
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
signalChanged("layoutChanged()", a);
};
void modelResetSlot
(
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
signalChanged("modelReset()", a);
};
signals:
void layoutChangedSignal
(
)
;
void modelResetSignal
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

if (signalSignature == "layoutChanged()")
{
emit layoutChangedSignal
(
);
return true;
}
if (signalSignature == "modelReset()")
{
emit modelResetSignal
(
);
return true;
}

if (QObjectAdapter::signalEmit(signalSignature, paramData))
  return true;

return false;
};
};
#endif // THEKLA_QABSTRACTITEMMODELADAPTER_H
