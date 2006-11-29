#ifndef THEKLA_QPROGRESSDIALOGADAPTER_H
#define THEKLA_QPROGRESSDIALOGADAPTER_H
#include "QDialogAdapter.h"
class QProgressDialogAdapter : public QDialogAdapter
{
Q_OBJECT
protected:
QProgressDialogAdapter(QtObject* owner)
 : QDialogAdapter(owner)
{};
public:
virtual ~QProgressDialogAdapter()
 {}; 
public:
static QtAdapter* create(QtObject* owner)
{
  return new QProgressDialogAdapter(owner);
};
public slots:
void canceledSlot
(
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
signalChanged("canceled()", a);
};
signals:
void canceledSignal
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

if (signalSignature == "canceled()")
{
emit canceledSignal
(
);
return true;
}

if (QDialogAdapter::signalEmit(signalSignature, paramData))
  return true;

return false;
};
};
#endif // THEKLA_QPROGRESSDIALOGADAPTER_H
