#ifndef THEKLA_QPROCESSADAPTER_H
#define THEKLA_QPROCESSADAPTER_H
#include "QIODeviceAdapter.h"
#include <QProcess>
class QProcessAdapter : public QIODeviceAdapter
{
Q_OBJECT
protected:
QProcessAdapter(QtObject* owner)
 : QIODeviceAdapter(owner)
{};
public:
virtual ~QProcessAdapter()
 {}; 
public:
static QtAdapter* create(QtObject* owner)
{
  return new QProcessAdapter(owner);
};
public slots:
void startedSlot
(
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
signalChanged("started()", a);
};
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
void readyReadStandardOutputSlot
(
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
signalChanged("readyReadStandardOutput()", a);
};
void readyReadStandardErrorSlot
(
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
signalChanged("readyReadStandardError()", a);
};
signals:
void startedSignal
(
)
;
void finishedSignal
(
int v0
)
;
void readyReadStandardOutputSignal
(
)
;
void readyReadStandardErrorSignal
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

if (signalSignature == "started()")
{
emit startedSignal
(
);
return true;
}
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
if (signalSignature == "readyReadStandardOutput()")
{
emit readyReadStandardOutputSignal
(
);
return true;
}
if (signalSignature == "readyReadStandardError()")
{
emit readyReadStandardErrorSignal
(
);
return true;
}

if (QIODeviceAdapter::signalEmit(signalSignature, paramData))
  return true;

return false;
};
};
#endif // THEKLA_QPROCESSADAPTER_H
