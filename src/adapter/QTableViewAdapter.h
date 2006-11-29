#ifndef THEKLA_QTABLEVIEWADAPTER_H
#define THEKLA_QTABLEVIEWADAPTER_H
#include "QAbstractItemViewAdapter.h"
class QTableViewAdapter : public QAbstractItemViewAdapter
{
Q_OBJECT
protected:
QTableViewAdapter(QtObject* owner)
 : QAbstractItemViewAdapter(owner)
{};
public:
virtual ~QTableViewAdapter()
 {}; 
public:
static QtAdapter* create(QtObject* owner)
{
  return new QTableViewAdapter(owner);
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
#endif // THEKLA_QTABLEVIEWADAPTER_H
