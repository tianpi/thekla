#ifndef THEKLA_QDATETIMEEDITADAPTER_H
#define THEKLA_QDATETIMEEDITADAPTER_H
#include "QAbstractSpinBoxAdapter.h"
#include <QDateTime>
#include <QTime>
#include <QDate>
class QDateTimeEditAdapter : public QAbstractSpinBoxAdapter
{
Q_OBJECT
protected:
QDateTimeEditAdapter(QtObject* owner)
 : QAbstractSpinBoxAdapter(owner)
{};
public:
virtual ~QDateTimeEditAdapter()
 {}; 
public:
static QtAdapter* create(QtObject* owner)
{
  return new QDateTimeEditAdapter(owner);
};
public slots:
void dateTimeChangedSlot
(
QDateTime v0
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
s << QVariant((QDateTime)v0);
signalChanged("dateTimeChanged(QDateTime)", a);
};
void timeChangedSlot
(
QTime v0
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
s << QVariant((QTime)v0);
signalChanged("timeChanged(QTime)", a);
};
void dateChangedSlot
(
QDate v0
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
s << QVariant((QDate)v0);
signalChanged("dateChanged(QDate)", a);
};
signals:
void dateTimeChangedSignal
(
QDateTime v0
)
;
void timeChangedSignal
(
QTime v0
)
;
void dateChangedSignal
(
QDate v0
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

if (signalSignature == "dateTimeChanged(QDateTime)")
{
QDateTime v0;
s >> v;
v0 = v.value<QDateTime>();
emit dateTimeChangedSignal
(
v0
);
return true;
}
if (signalSignature == "timeChanged(QTime)")
{
QTime v0;
s >> v;
v0 = v.value<QTime>();
emit timeChangedSignal
(
v0
);
return true;
}
if (signalSignature == "dateChanged(QDate)")
{
QDate v0;
s >> v;
v0 = v.value<QDate>();
emit dateChangedSignal
(
v0
);
return true;
}

if (QAbstractSpinBoxAdapter::signalEmit(signalSignature, paramData))
  return true;

return false;
};
};
#endif // THEKLA_QDATETIMEEDITADAPTER_H
