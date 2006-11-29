#ifndef THEKLA_QDOUBLESPINBOXADAPTER_H
#define THEKLA_QDOUBLESPINBOXADAPTER_H
#include "QAbstractSpinBoxAdapter.h"
#include <QString>
class QDoubleSpinBoxAdapter : public QAbstractSpinBoxAdapter
{
Q_OBJECT
protected:
QDoubleSpinBoxAdapter(QtObject* owner)
 : QAbstractSpinBoxAdapter(owner)
{};
public:
virtual ~QDoubleSpinBoxAdapter()
 {}; 
public:
static QtAdapter* create(QtObject* owner)
{
  return new QDoubleSpinBoxAdapter(owner);
};
public slots:
void valueChangedSlot
(
double v0
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
s << QVariant((double)v0);
signalChanged("valueChanged(double)", a);
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
double v0
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

if (signalSignature == "valueChanged(double)")
{
double v0;
s >> v;
v0 = v.value<double>();
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
#endif // THEKLA_QDOUBLESPINBOXADAPTER_H
