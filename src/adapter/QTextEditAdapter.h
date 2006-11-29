#ifndef THEKLA_QTEXTEDITADAPTER_H
#define THEKLA_QTEXTEDITADAPTER_H
#include "QAbstractScrollAreaAdapter.h"
#include <QTextCharFormat>
#include <QFont>
#include <QColor>
class QTextEditAdapter : public QAbstractScrollAreaAdapter
{
Q_OBJECT
protected:
QTextEditAdapter(QtObject* owner)
 : QAbstractScrollAreaAdapter(owner)
{};
public:
virtual ~QTextEditAdapter()
 {}; 
public:
static QtAdapter* create(QtObject* owner)
{
  return new QTextEditAdapter(owner);
};
public slots:
void textChangedSlot
(
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
signalChanged("textChanged()", a);
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
void copyAvailableSlot
(
bool v0
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
s << QVariant((bool)v0);
signalChanged("copyAvailable(bool)", a);
};
void selectionChangedSlot
(
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
signalChanged("selectionChanged()", a);
};
void cursorPositionChangedSlot
(
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
signalChanged("cursorPositionChanged()", a);
};
void currentFontChangedSlot
(
QFont v0
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
s << QVariant((QFont)v0);
signalChanged("currentFontChanged(QFont)", a);
};
void currentColorChangedSlot
(
QColor v0
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
s << QVariant((QColor)v0);
signalChanged("currentColorChanged(QColor)", a);
};
signals:
void textChangedSignal
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
void copyAvailableSignal
(
bool v0
)
;
void selectionChangedSignal
(
)
;
void cursorPositionChangedSignal
(
)
;
void currentFontChangedSignal
(
QFont v0
)
;
void currentColorChangedSignal
(
QColor v0
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

if (signalSignature == "textChanged()")
{
emit textChangedSignal
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
if (signalSignature == "copyAvailable(bool)")
{
bool v0;
s >> v;
v0 = v.value<bool>();
emit copyAvailableSignal
(
v0
);
return true;
}
if (signalSignature == "selectionChanged()")
{
emit selectionChangedSignal
(
);
return true;
}
if (signalSignature == "cursorPositionChanged()")
{
emit cursorPositionChangedSignal
(
);
return true;
}
if (signalSignature == "currentFontChanged(QFont)")
{
QFont v0;
s >> v;
v0 = v.value<QFont>();
emit currentFontChangedSignal
(
v0
);
return true;
}
if (signalSignature == "currentColorChanged(QColor)")
{
QColor v0;
s >> v;
v0 = v.value<QColor>();
emit currentColorChangedSignal
(
v0
);
return true;
}

if (QAbstractScrollAreaAdapter::signalEmit(signalSignature, paramData))
  return true;

return false;
};
};
#endif // THEKLA_QTEXTEDITADAPTER_H
