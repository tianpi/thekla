#ifndef THEKLA_QPROGRESSBARADAPTER_H
#define THEKLA_QPROGRESSBARADAPTER_H
#include "QWidgetAdapter.h"
class QProgressBarAdapter : public QWidgetAdapter
{
Q_OBJECT
protected:
QProgressBarAdapter(QtObject* owner)
 : QWidgetAdapter(owner)
{};
public:
virtual ~QProgressBarAdapter()
 {}; 
public:
static QtAdapter* create(QtObject* owner)
{
  return new QProgressBarAdapter(owner);
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
signals:
void valueChangedSignal
(
int v0
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

if (QWidgetAdapter::signalEmit(signalSignature, paramData))
  return true;

return false;
};
};
#endif // THEKLA_QPROGRESSBARADAPTER_H
