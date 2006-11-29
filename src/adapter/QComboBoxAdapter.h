#ifndef THEKLA_QCOMBOBOXADAPTER_H
#define THEKLA_QCOMBOBOXADAPTER_H
#include "QWidgetAdapter.h"
#include <QString>
class QComboBoxAdapter : public QWidgetAdapter
{
Q_OBJECT
protected:
QComboBoxAdapter(QtObject* owner)
 : QWidgetAdapter(owner)
{};
public:
virtual ~QComboBoxAdapter()
 {}; 
public:
static QtAdapter* create(QtObject* owner)
{
  return new QComboBoxAdapter(owner);
};
public slots:
void editTextChangedSlot
(
QString v0
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
s << QVariant((QString)v0);
signalChanged("editTextChanged(QString)", a);
};
void activatedSlot
(
int v0
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
s << QVariant((int)v0);
signalChanged("activated(int)", a);
};
void activatedSlot
(
QString v0
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
s << QVariant((QString)v0);
signalChanged("activated(QString)", a);
};
void highlightedSlot
(
int v0
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
s << QVariant((int)v0);
signalChanged("highlighted(int)", a);
};
void highlightedSlot
(
QString v0
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
s << QVariant((QString)v0);
signalChanged("highlighted(QString)", a);
};
void currentIndexChangedSlot
(
int v0
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
s << QVariant((int)v0);
signalChanged("currentIndexChanged(int)", a);
};
void currentIndexChangedSlot
(
QString v0
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
s << QVariant((QString)v0);
signalChanged("currentIndexChanged(QString)", a);
};
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
signals:
void editTextChangedSignal
(
QString v0
)
;
void activatedSignal
(
int v0
)
;
void activatedSignal
(
QString v0
)
;
void highlightedSignal
(
int v0
)
;
void highlightedSignal
(
QString v0
)
;
void currentIndexChangedSignal
(
int v0
)
;
void currentIndexChangedSignal
(
QString v0
)
;
void textChangedSignal
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

if (signalSignature == "editTextChanged(QString)")
{
QString v0;
s >> v;
v0 = v.value<QString>();
emit editTextChangedSignal
(
v0
);
return true;
}
if (signalSignature == "activated(int)")
{
int v0;
s >> v;
v0 = v.value<int>();
emit activatedSignal
(
v0
);
return true;
}
if (signalSignature == "activated(QString)")
{
QString v0;
s >> v;
v0 = v.value<QString>();
emit activatedSignal
(
v0
);
return true;
}
if (signalSignature == "highlighted(int)")
{
int v0;
s >> v;
v0 = v.value<int>();
emit highlightedSignal
(
v0
);
return true;
}
if (signalSignature == "highlighted(QString)")
{
QString v0;
s >> v;
v0 = v.value<QString>();
emit highlightedSignal
(
v0
);
return true;
}
if (signalSignature == "currentIndexChanged(int)")
{
int v0;
s >> v;
v0 = v.value<int>();
emit currentIndexChangedSignal
(
v0
);
return true;
}
if (signalSignature == "currentIndexChanged(QString)")
{
QString v0;
s >> v;
v0 = v.value<QString>();
emit currentIndexChangedSignal
(
v0
);
return true;
}
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

if (QWidgetAdapter::signalEmit(signalSignature, paramData))
  return true;

return false;
};
};
#endif // THEKLA_QCOMBOBOXADAPTER_H
