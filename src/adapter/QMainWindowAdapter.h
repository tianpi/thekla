#ifndef THEKLA_QMAINWINDOWADAPTER_H
#define THEKLA_QMAINWINDOWADAPTER_H
#include "QWidgetAdapter.h"
#include <QSize>
#include <Qt>
class QMainWindowAdapter : public QWidgetAdapter
{
Q_OBJECT
protected:
QMainWindowAdapter(QtObject* owner)
 : QWidgetAdapter(owner)
{};
public:
virtual ~QMainWindowAdapter()
 {}; 
public:
static QtAdapter* create(QtObject* owner)
{
  return new QMainWindowAdapter(owner);
};
public slots:
void iconSizeChangedSlot
(
QSize v0
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
s << QVariant((QSize)v0);
signalChanged("iconSizeChanged(QSize)", a);
};
signals:
void iconSizeChangedSignal
(
QSize v0
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

if (signalSignature == "iconSizeChanged(QSize)")
{
QSize v0;
s >> v;
v0 = v.value<QSize>();
emit iconSizeChangedSignal
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
#endif // THEKLA_QMAINWINDOWADAPTER_H
