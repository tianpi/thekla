#ifndef THEKLA_QWIDGETADAPTER_H
#define THEKLA_QWIDGETADAPTER_H
#include "QObjectAdapter.h"
#include <QPoint>
class QWidgetAdapter : public QObjectAdapter
{
Q_OBJECT
protected:
QWidgetAdapter(QtObject* owner)
 : QObjectAdapter(owner)
{};
public:
virtual ~QWidgetAdapter()
 {}; 
public:
static QtAdapter* create(QtObject* owner)
{
  return new QWidgetAdapter(owner);
};
public slots:
void customContextMenuRequestedSlot
(
QPoint v0
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
s << QVariant((QPoint)v0);
signalChanged("customContextMenuRequested(QPoint)", a);
};
signals:
void customContextMenuRequestedSignal
(
QPoint v0
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

if (signalSignature == "customContextMenuRequested(QPoint)")
{
QPoint v0;
s >> v;
v0 = v.value<QPoint>();
emit customContextMenuRequestedSignal
(
v0
);
return true;
}

if (QObjectAdapter::signalEmit(signalSignature, paramData))
  return true;

return false;
};
};
#endif // THEKLA_QWIDGETADAPTER_H
