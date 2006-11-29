#ifndef THEKLA_QTREEVIEWADAPTER_H
#define THEKLA_QTREEVIEWADAPTER_H
#include "QAbstractItemViewAdapter.h"
#include <QModelIndex>
class QTreeViewAdapter : public QAbstractItemViewAdapter
{
Q_OBJECT
protected:
QTreeViewAdapter(QtObject* owner)
 : QAbstractItemViewAdapter(owner)
{};
public:
virtual ~QTreeViewAdapter()
 {}; 
public:
static QtAdapter* create(QtObject* owner)
{
  return new QTreeViewAdapter(owner);
};
public slots:
signals:
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


if (QAbstractItemViewAdapter::signalEmit(signalSignature, paramData))
  return true;

return false;
};
};
#endif // THEKLA_QTREEVIEWADAPTER_H
