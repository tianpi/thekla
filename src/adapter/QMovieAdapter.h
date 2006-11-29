#ifndef THEKLA_QMOVIEADAPTER_H
#define THEKLA_QMOVIEADAPTER_H
#include "QObjectAdapter.h"
#include <QSize>
#include <QRect>
#include <QMovie>
#include <QImageReader>
class QMovieAdapter : public QObjectAdapter
{
Q_OBJECT
protected:
QMovieAdapter(QtObject* owner)
 : QObjectAdapter(owner)
{};
public:
virtual ~QMovieAdapter()
 {}; 
public:
static QtAdapter* create(QtObject* owner)
{
  return new QMovieAdapter(owner);
};
public slots:
void startedSlot
(
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
signalChanged("started()", a);
};
void resizedSlot
(
QSize v0
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
s << QVariant((QSize)v0);
signalChanged("resized(QSize)", a);
};
void updatedSlot
(
QRect v0
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
s << QVariant((QRect)v0);
signalChanged("updated(QRect)", a);
};
void finishedSlot
(
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
signalChanged("finished()", a);
};
void frameChangedSlot
(
int v0
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
s << QVariant((int)v0);
signalChanged("frameChanged(int)", a);
};
signals:
void startedSignal
(
)
;
void resizedSignal
(
QSize v0
)
;
void updatedSignal
(
QRect v0
)
;
void finishedSignal
(
)
;
void frameChangedSignal
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

if (signalSignature == "started()")
{
emit startedSignal
(
);
return true;
}
if (signalSignature == "resized(QSize)")
{
QSize v0;
s >> v;
v0 = v.value<QSize>();
emit resizedSignal
(
v0
);
return true;
}
if (signalSignature == "updated(QRect)")
{
QRect v0;
s >> v;
v0 = v.value<QRect>();
emit updatedSignal
(
v0
);
return true;
}
if (signalSignature == "finished()")
{
emit finishedSignal
(
);
return true;
}
if (signalSignature == "frameChanged(int)")
{
int v0;
s >> v;
v0 = v.value<int>();
emit frameChangedSignal
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
#endif // THEKLA_QMOVIEADAPTER_H
