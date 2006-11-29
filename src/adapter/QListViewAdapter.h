#ifndef THEKLA_QLISTVIEWADAPTER_H
#define THEKLA_QLISTVIEWADAPTER_H
#include "QAbstractItemViewAdapter.h"
class QListViewAdapter : public QAbstractItemViewAdapter
{
Q_OBJECT
protected:
QListViewAdapter(QtObject* owner)
 : QAbstractItemViewAdapter(owner)
{};
public:
virtual ~QListViewAdapter()
 {}; 
public:
static QtAdapter* create(QtObject* owner)
{
  return new QListViewAdapter(owner);
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
#endif // THEKLA_QLISTVIEWADAPTER_H
