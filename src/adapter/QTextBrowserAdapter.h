#ifndef THEKLA_QTEXTBROWSERADAPTER_H
#define THEKLA_QTEXTBROWSERADAPTER_H
#include "QTextEditAdapter.h"
#include <QUrl>
#include <QString>
class QTextBrowserAdapter : public QTextEditAdapter
{
Q_OBJECT
protected:
QTextBrowserAdapter(QtObject* owner)
 : QTextEditAdapter(owner)
{};
public:
virtual ~QTextBrowserAdapter()
 {}; 
public:
static QtAdapter* create(QtObject* owner)
{
  return new QTextBrowserAdapter(owner);
};
public slots:
void backwardAvailableSlot
(
bool v0
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
s << QVariant((bool)v0);
signalChanged("backwardAvailable(bool)", a);
};
void forwardAvailableSlot
(
bool v0
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
s << QVariant((bool)v0);
signalChanged("forwardAvailable(bool)", a);
};
void sourceChangedSlot
(
QUrl v0
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
s << QVariant((QUrl)v0);
signalChanged("sourceChanged(QUrl)", a);
};
void highlightedSlot
(
QUrl v0
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
s << QVariant((QUrl)v0);
signalChanged("highlighted(QUrl)", a);
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
void anchorClickedSlot
(
QUrl v0
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
s << QVariant((QUrl)v0);
signalChanged("anchorClicked(QUrl)", a);
};
signals:
void backwardAvailableSignal
(
bool v0
)
;
void forwardAvailableSignal
(
bool v0
)
;
void sourceChangedSignal
(
QUrl v0
)
;
void highlightedSignal
(
QUrl v0
)
;
void highlightedSignal
(
QString v0
)
;
void anchorClickedSignal
(
QUrl v0
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

if (signalSignature == "backwardAvailable(bool)")
{
bool v0;
s >> v;
v0 = v.value<bool>();
emit backwardAvailableSignal
(
v0
);
return true;
}
if (signalSignature == "forwardAvailable(bool)")
{
bool v0;
s >> v;
v0 = v.value<bool>();
emit forwardAvailableSignal
(
v0
);
return true;
}
if (signalSignature == "sourceChanged(QUrl)")
{
QUrl v0;
s >> v;
v0 = v.value<QUrl>();
emit sourceChangedSignal
(
v0
);
return true;
}
if (signalSignature == "highlighted(QUrl)")
{
QUrl v0;
s >> v;
v0 = v.value<QUrl>();
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
if (signalSignature == "anchorClicked(QUrl)")
{
QUrl v0;
s >> v;
v0 = v.value<QUrl>();
emit anchorClickedSignal
(
v0
);
return true;
}

if (QTextEditAdapter::signalEmit(signalSignature, paramData))
  return true;

return false;
};
};
#endif // THEKLA_QTEXTBROWSERADAPTER_H
