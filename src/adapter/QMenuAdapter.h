#ifndef THEKLA_QMENUADAPTER_H
#define THEKLA_QMENUADAPTER_H
#include "QWidgetAdapter.h"
#include <QAction>
class QMenuAdapter : public QWidgetAdapter
{
Q_OBJECT
protected:
QMenuAdapter(QtObject* owner)
 : QWidgetAdapter(owner)
{};
public:
virtual ~QMenuAdapter()
 {}; 
public:
static QtAdapter* create(QtObject* owner)
{
  return new QMenuAdapter(owner);
};
public slots:
void aboutToShowSlot
(
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
signalChanged("aboutToShow()", a);
};
void aboutToHideSlot
(
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
signalChanged("aboutToHide()", a);
};
void activatedSlot
(
int v0
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
s << QVariant((int)v0);
signalChanged("activated(int)", a);
};
void highlightedSlot
(
int v0
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
s << QVariant((int)v0);
signalChanged("highlighted(int)", a);
};
signals:
void aboutToShowSignal
(
)
;
void aboutToHideSignal
(
)
;
void activatedSignal
(
int v0
)
;
void highlightedSignal
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

if (signalSignature == "aboutToShow()")
{
emit aboutToShowSignal
(
);
return true;
}
if (signalSignature == "aboutToHide()")
{
emit aboutToHideSignal
(
);
return true;
}
if (signalSignature == "activated(int)")
{
int v0;
s >> v;
v0 = v.value<int>();
emit activatedSignal
(
v0
);
return true;
}
if (signalSignature == "highlighted(int)")
{
int v0;
s >> v;
v0 = v.value<int>();
emit highlightedSignal
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
#endif // THEKLA_QMENUADAPTER_H
