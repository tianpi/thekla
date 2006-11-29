#ifndef THEKLA_QDIALOGADAPTER_H
#define THEKLA_QDIALOGADAPTER_H
#include "QWidgetAdapter.h"
class QDialogAdapter : public QWidgetAdapter
{
Q_OBJECT
protected:
QDialogAdapter(QtObject* owner)
 : QWidgetAdapter(owner)
{};
public:
virtual ~QDialogAdapter()
 {}; 
public:
static QtAdapter* create(QtObject* owner)
{
  return new QDialogAdapter(owner);
};
public slots:
void finishedSlot
(
int v0
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
s << QVariant((int)v0);
signalChanged("finished(int)", a);
};
void acceptedSlot
(
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
signalChanged("accepted()", a);
};
void rejectedSlot
(
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
signalChanged("rejected()", a);
};
signals:
void finishedSignal
(
int v0
)
;
void acceptedSignal
(
)
;
void rejectedSignal
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

if (signalSignature == "finished(int)")
{
int v0;
s >> v;
v0 = v.value<int>();
emit finishedSignal
(
v0
);
return true;
}
if (signalSignature == "accepted()")
{
emit acceptedSignal
(
);
return true;
}
if (signalSignature == "rejected()")
{
emit rejectedSignal
(
);
return true;
}

if (QWidgetAdapter::signalEmit(signalSignature, paramData))
  return true;

return false;
};
};
#endif // THEKLA_QDIALOGADAPTER_H
