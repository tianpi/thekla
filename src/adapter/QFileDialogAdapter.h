#ifndef THEKLA_QFILEDIALOGADAPTER_H
#define THEKLA_QFILEDIALOGADAPTER_H
#include "QDialogAdapter.h"
#include <QStringList>
#include <QString>
class QFileDialogAdapter : public QDialogAdapter
{
Q_OBJECT
protected:
QFileDialogAdapter(QtObject* owner)
 : QDialogAdapter(owner)
{};
public:
virtual ~QFileDialogAdapter()
 {}; 
public:
static QtAdapter* create(QtObject* owner)
{
  return new QFileDialogAdapter(owner);
};
public slots:
void filesSelectedSlot
(
QStringList v0
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
s << QVariant((QStringList)v0);
signalChanged("filesSelected(QStringList)", a);
};
void currentChangedSlot
(
QString v0
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
s << QVariant((QString)v0);
signalChanged("currentChanged(QString)", a);
};
signals:
void filesSelectedSignal
(
QStringList v0
)
;
void currentChangedSignal
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

if (signalSignature == "filesSelected(QStringList)")
{
QStringList v0;
s >> v;
v0 = v.value<QStringList>();
emit filesSelectedSignal
(
v0
);
return true;
}
if (signalSignature == "currentChanged(QString)")
{
QString v0;
s >> v;
v0 = v.value<QString>();
emit currentChangedSignal
(
v0
);
return true;
}

if (QDialogAdapter::signalEmit(signalSignature, paramData))
  return true;

return false;
};
};
#endif // THEKLA_QFILEDIALOGADAPTER_H
