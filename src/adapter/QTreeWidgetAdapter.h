#ifndef THEKLA_QTREEWIDGETADAPTER_H
#define THEKLA_QTREEWIDGETADAPTER_H
#include "QTreeViewAdapter.h"
#include <QTreeWidgetItem>
class QTreeWidgetAdapter : public QTreeViewAdapter
{
Q_OBJECT
protected:
QTreeWidgetAdapter(QtObject* owner)
 : QTreeViewAdapter(owner)
{};
public:
virtual ~QTreeWidgetAdapter()
 {}; 
public:
static QtAdapter* create(QtObject* owner)
{
  return new QTreeWidgetAdapter(owner);
};
public slots:
void itemSelectionChangedSlot
(
)
{
QByteArray a;
QDataStream s(&a, QIODevice::WriteOnly);
signalChanged("itemSelectionChanged()", a);
};
signals:
void itemSelectionChangedSignal
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

if (signalSignature == "itemSelectionChanged()")
{
emit itemSelectionChangedSignal
(
);
return true;
}

if (QTreeViewAdapter::signalEmit(signalSignature, paramData))
  return true;

return false;
};
};
#endif // THEKLA_QTREEWIDGETADAPTER_H
