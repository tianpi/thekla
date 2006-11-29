#ifndef THEKLA_QTEXTDOCUMENTADAPTER_H
#define THEKLA_QTEXTDOCUMENTADAPTER_H
#include "QObjectAdapter.h"
#include <QTextCursor>
class QTextDocumentAdapter : public QObjectAdapter
{
Q_OBJECT
protected:
QTextDocumentAdapter(QtObject* owner)
 : QObjectAdapter(owner)
{};
public:
virtual ~QTextDocumentAdapter()
 {}; 
public:
static QtAdapter* create(QtObject* owner)
{
  return new QTextDocumentAdapter(owner);
};
public slots:
void contentsChangeSlot
(
int v0
, 
int v1
, 
int v2
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
s << QVariant((int)v0);
s << QVariant((int)v1);
s << QVariant((int)v2);
signalChanged("contentsChange(int,int,int)", a);
};
void contentsChangedSlot
(
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
signalChanged("contentsChanged()", a);
};
void undoAvailableSlot
(
bool v0
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
s << QVariant((bool)v0);
signalChanged("undoAvailable(bool)", a);
};
void redoAvailableSlot
(
bool v0
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
s << QVariant((bool)v0);
signalChanged("redoAvailable(bool)", a);
};
void modificationChangedSlot
(
bool v0
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
s << QVariant((bool)v0);
signalChanged("modificationChanged(bool)", a);
};
signals:
void contentsChangeSignal
(
int v0
, 
int v1
, 
int v2
)
;
void contentsChangedSignal
(
)
;
void undoAvailableSignal
(
bool v0
)
;
void redoAvailableSignal
(
bool v0
)
;
void modificationChangedSignal
(
bool v0
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

if (signalSignature == "contentsChange(int,int,int)")
{
int v0;
s >> v;
v0 = v.value<int>();
int v1;
s >> v;
v1 = v.value<int>();
int v2;
s >> v;
v2 = v.value<int>();
emit contentsChangeSignal
(
v0
, 
v1
, 
v2
);
return true;
}
if (signalSignature == "contentsChanged()")
{
emit contentsChangedSignal
(
);
return true;
}
if (signalSignature == "undoAvailable(bool)")
{
bool v0;
s >> v;
v0 = v.value<bool>();
emit undoAvailableSignal
(
v0
);
return true;
}
if (signalSignature == "redoAvailable(bool)")
{
bool v0;
s >> v;
v0 = v.value<bool>();
emit redoAvailableSignal
(
v0
);
return true;
}
if (signalSignature == "modificationChanged(bool)")
{
bool v0;
s >> v;
v0 = v.value<bool>();
emit modificationChangedSignal
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
#endif // THEKLA_QTEXTDOCUMENTADAPTER_H
