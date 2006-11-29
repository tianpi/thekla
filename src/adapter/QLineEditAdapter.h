#ifndef THEKLA_QLINEEDITADAPTER_H
#define THEKLA_QLINEEDITADAPTER_H
#include "QWidgetAdapter.h"
#include <QString>
class QLineEditAdapter : public QWidgetAdapter
{
Q_OBJECT
protected:
QLineEditAdapter(QtObject* owner)
 : QWidgetAdapter(owner)
{};
public:
virtual ~QLineEditAdapter()
 {}; 
public:
static QtAdapter* create(QtObject* owner)
{
  return new QLineEditAdapter(owner);
};
public slots:
void textChangedSlot
(
QString v0
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
s << QVariant((QString)v0);
signalChanged("textChanged(QString)", a);
};
void textEditedSlot
(
QString v0
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
s << QVariant((QString)v0);
signalChanged("textEdited(QString)", a);
};
void cursorPositionChangedSlot
(
int v0
, 
int v1
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
s << QVariant((int)v0);
s << QVariant((int)v1);
signalChanged("cursorPositionChanged(int,int)", a);
};
void returnPressedSlot
(
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
signalChanged("returnPressed()", a);
};
void editingFinishedSlot
(
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
signalChanged("editingFinished()", a);
};
void selectionChangedSlot
(
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
signalChanged("selectionChanged()", a);
};
void lostFocusSlot
(
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
signalChanged("lostFocus()", a);
};
signals:
void textChangedSignal
(
QString v0
)
;
void textEditedSignal
(
QString v0
)
;
void cursorPositionChangedSignal
(
int v0
, 
int v1
)
;
void returnPressedSignal
(
)
;
void editingFinishedSignal
(
)
;
void selectionChangedSignal
(
)
;
void lostFocusSignal
(
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

if (signalSignature == "textChanged(QString)")
{
QString v0;
s >> v;
v0 = v.value<QString>();
emit textChangedSignal
(
v0
);
return true;
}
if (signalSignature == "textEdited(QString)")
{
QString v0;
s >> v;
v0 = v.value<QString>();
emit textEditedSignal
(
v0
);
return true;
}
if (signalSignature == "cursorPositionChanged(int,int)")
{
int v0;
s >> v;
v0 = v.value<int>();
int v1;
s >> v;
v1 = v.value<int>();
emit cursorPositionChangedSignal
(
v0
, 
v1
);
return true;
}
if (signalSignature == "returnPressed()")
{
emit returnPressedSignal
(
);
return true;
}
if (signalSignature == "editingFinished()")
{
emit editingFinishedSignal
(
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
if (signalSignature == "lostFocus()")
{
emit lostFocusSignal
(
);
return true;
}

if (QWidgetAdapter::signalEmit(signalSignature, paramData))
  return true;

return false;
};
};
#endif // THEKLA_QLINEEDITADAPTER_H
