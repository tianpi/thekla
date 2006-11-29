#ifndef THEKLA_QMENUBARADAPTER_H
#define THEKLA_QMENUBARADAPTER_H
#include "QWidgetAdapter.h"
#include <QAction>
class QMenuBarAdapter : public QWidgetAdapter
{
Q_OBJECT
protected:
QMenuBarAdapter(QtObject* owner)
 : QWidgetAdapter(owner)
{};
public:
virtual ~QMenuBarAdapter()
 {}; 
public:
static QtAdapter* create(QtObject* owner)
{
  return new QMenuBarAdapter(owner);
};
public slots:
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
#endif // THEKLA_QMENUBARADAPTER_H
