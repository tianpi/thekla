#ifndef THEKLA_QSTATUSBARADAPTER_H
#define THEKLA_QSTATUSBARADAPTER_H
#include "QWidgetAdapter.h"
#include <QString>
class QStatusBarAdapter : public QWidgetAdapter
{
Q_OBJECT
protected:
QStatusBarAdapter(QtObject* owner)
 : QWidgetAdapter(owner)
{};
public:
virtual ~QStatusBarAdapter()
 {}; 
public:
static QtAdapter* create(QtObject* owner)
{
  return new QStatusBarAdapter(owner);
};
public slots:
void messageChangedSlot
(
QString v0
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
s << QVariant((QString)v0);
signalChanged("messageChanged(QString)", a);
};
signals:
void messageChangedSignal
(
QString v0
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

if (signalSignature == "messageChanged(QString)")
{
QString v0;
s >> v;
v0 = v.value<QString>();
emit messageChangedSignal
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
#endif // THEKLA_QSTATUSBARADAPTER_H
