#ifndef THEKLA_QBUTTONGROUPADAPTER_H
#define THEKLA_QBUTTONGROUPADAPTER_H
#include "QObjectAdapter.h"
#include <QAbstractButton>
class QButtonGroupAdapter : public QObjectAdapter
{
Q_OBJECT
protected:
QButtonGroupAdapter(QtObject* owner)
 : QObjectAdapter(owner)
{};
public:
virtual ~QButtonGroupAdapter()
 {}; 
public:
static QtAdapter* create(QtObject* owner)
{
  return new QButtonGroupAdapter(owner);
};
public slots:
void buttonClickedSlot
(
int v0
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
s << QVariant((int)v0);
signalChanged("buttonClicked(int)", a);
};
signals:
void buttonClickedSignal
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

if (signalSignature == "buttonClicked(int)")
{
int v0;
s >> v;
v0 = v.value<int>();
emit buttonClickedSignal
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
#endif // THEKLA_QBUTTONGROUPADAPTER_H
