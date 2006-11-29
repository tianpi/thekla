#ifndef THEKLA_QSPINBOXADAPTER_H
#define THEKLA_QSPINBOXADAPTER_H
#include "QAbstractSpinBoxAdapter.h"
#include <QString>
class QSpinBoxAdapter : public QAbstractSpinBoxAdapter
{
Q_OBJECT
protected:
QSpinBoxAdapter(QtObject* owner)
 : QAbstractSpinBoxAdapter(owner)
{};
public:
virtual ~QSpinBoxAdapter()
 {}; 
public:
static QtAdapter* create(QtObject* owner)
{
  return new QSpinBoxAdapter(owner);
};
public slots:
void valueChangedSlot
(
int v0
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
s << QVariant((int)v0);
signalChanged("valueChanged(int)", a);
};
void valueChangedSlot
(
QString v0
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
s << QVariant((QString)v0);
signalChanged("valueChanged(QString)", a);
};
signals:
void valueChangedSignal
(
int v0
)
;
void valueChangedSignal
(
QString v0
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

if (signalSignature == "valueChanged(int)")
{
int v0;
s >> v;
v0 = v.value<int>();
emit valueChangedSignal
(
v0
);
return true;
}
if (signalSignature == "valueChanged(QString)")
{
QString v0;
s >> v;
v0 = v.value<QString>();
emit valueChangedSignal
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
#endif // THEKLA_QSPINBOXADAPTER_H
