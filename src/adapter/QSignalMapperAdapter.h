#ifndef THEKLA_QSIGNALMAPPERADAPTER_H
#define THEKLA_QSIGNALMAPPERADAPTER_H
#include "QObjectAdapter.h"
#include <QString>
#include <QWidget>
#include <QObject>
class QSignalMapperAdapter : public QObjectAdapter
{
Q_OBJECT
protected:
QSignalMapperAdapter(QtObject* owner)
 : QObjectAdapter(owner)
{};
public:
virtual ~QSignalMapperAdapter()
 {}; 
public:
static QtAdapter* create(QtObject* owner)
{
  return new QSignalMapperAdapter(owner);
};
public slots:
void mappedSlot
(
int v0
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
s << QVariant((int)v0);
signalChanged("mapped(int)", a);
};
void mappedSlot
(
QString v0
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
s << QVariant((QString)v0);
signalChanged("mapped(QString)", a);
};
signals:
void mappedSignal
(
int v0
)
;
void mappedSignal
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

if (signalSignature == "mapped(int)")
{
int v0;
s >> v;
v0 = v.value<int>();
emit mappedSignal
(
v0
);
return true;
}
if (signalSignature == "mapped(QString)")
{
QString v0;
s >> v;
v0 = v.value<QString>();
emit mappedSignal
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
#endif // THEKLA_QSIGNALMAPPERADAPTER_H
