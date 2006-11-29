#ifndef THEKLA_QTABWIDGETADAPTER_H
#define THEKLA_QTABWIDGETADAPTER_H
#include "QWidgetAdapter.h"
#include <QWidget>
class QTabWidgetAdapter : public QWidgetAdapter
{
Q_OBJECT
protected:
QTabWidgetAdapter(QtObject* owner)
 : QWidgetAdapter(owner)
{};
public:
virtual ~QTabWidgetAdapter()
 {}; 
public:
static QtAdapter* create(QtObject* owner)
{
  return new QTabWidgetAdapter(owner);
};
public slots:
void currentChangedSlot
(
int v0
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
s << QVariant((int)v0);
signalChanged("currentChanged(int)", a);
};
signals:
void currentChangedSignal
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

if (signalSignature == "currentChanged(int)")
{
int v0;
s >> v;
v0 = v.value<int>();
emit currentChangedSignal
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
#endif // THEKLA_QTABWIDGETADAPTER_H
