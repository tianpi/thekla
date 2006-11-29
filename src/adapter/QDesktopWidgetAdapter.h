#ifndef THEKLA_QDESKTOPWIDGETADAPTER_H
#define THEKLA_QDESKTOPWIDGETADAPTER_H
#include "QWidgetAdapter.h"
class QDesktopWidgetAdapter : public QWidgetAdapter
{
Q_OBJECT
protected:
QDesktopWidgetAdapter(QtObject* owner)
 : QWidgetAdapter(owner)
{};
public:
virtual ~QDesktopWidgetAdapter()
 {}; 
public:
static QtAdapter* create(QtObject* owner)
{
  return new QDesktopWidgetAdapter(owner);
};
public slots:
void resizedSlot
(
int v0
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
s << QVariant((int)v0);
signalChanged("resized(int)", a);
};
void workAreaResizedSlot
(
int v0
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
s << QVariant((int)v0);
signalChanged("workAreaResized(int)", a);
};
signals:
void resizedSignal
(
int v0
)
;
void workAreaResizedSignal
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

if (signalSignature == "resized(int)")
{
int v0;
s >> v;
v0 = v.value<int>();
emit resizedSignal
(
v0
);
return true;
}
if (signalSignature == "workAreaResized(int)")
{
int v0;
s >> v;
v0 = v.value<int>();
emit workAreaResizedSignal
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
#endif // THEKLA_QDESKTOPWIDGETADAPTER_H
