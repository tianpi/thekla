#ifndef THEKLA_QGROUPBOXADAPTER_H
#define THEKLA_QGROUPBOXADAPTER_H
#include "QWidgetAdapter.h"
class QGroupBoxAdapter : public QWidgetAdapter
{
Q_OBJECT
protected:
QGroupBoxAdapter(QtObject* owner)
 : QWidgetAdapter(owner)
{};
public:
virtual ~QGroupBoxAdapter()
 {}; 
public:
static QtAdapter* create(QtObject* owner)
{
  return new QGroupBoxAdapter(owner);
};
public slots:
void toggledSlot
(
bool v0
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
s << QVariant((bool)v0);
signalChanged("toggled(bool)", a);
};
signals:
void toggledSignal
(
bool v0
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

if (signalSignature == "toggled(bool)")
{
bool v0;
s >> v;
v0 = v.value<bool>();
emit toggledSignal
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
#endif // THEKLA_QGROUPBOXADAPTER_H
