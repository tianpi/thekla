#ifndef THEKLA_QTOOLBARADAPTER_H
#define THEKLA_QTOOLBARADAPTER_H
#include "QWidgetAdapter.h"
#include <QAction>
#include <Qt>
#include <QSize>
class QToolBarAdapter : public QWidgetAdapter
{
Q_OBJECT
protected:
QToolBarAdapter(QtObject* owner)
 : QWidgetAdapter(owner)
{};
public:
virtual ~QToolBarAdapter()
 {}; 
public:
static QtAdapter* create(QtObject* owner)
{
  return new QToolBarAdapter(owner);
};
public slots:
void movableChangedSlot
(
bool v0
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
s << QVariant((bool)v0);
signalChanged("movableChanged(bool)", a);
};
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
void movableChangedSignal
(
bool v0
)
;
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

if (signalSignature == "movableChanged(bool)")
{
bool v0;
s >> v;
v0 = v.value<bool>();
emit movableChangedSignal
(
v0
);
return true;
}
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
#endif // THEKLA_QTOOLBARADAPTER_H
