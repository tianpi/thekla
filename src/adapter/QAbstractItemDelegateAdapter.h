#ifndef THEKLA_QABSTRACTITEMDELEGATEADAPTER_H
#define THEKLA_QABSTRACTITEMDELEGATEADAPTER_H
#include "QObjectAdapter.h"
#include <QWidget>
#include <QAbstractItemDelegate>
class QAbstractItemDelegateAdapter : public QObjectAdapter
{
Q_OBJECT
protected:
QAbstractItemDelegateAdapter(QtObject* owner)
 : QObjectAdapter(owner)
{};
public:
virtual ~QAbstractItemDelegateAdapter()
 {}; 
public:
static QtAdapter* create(QtObject* owner)
{
  return new QAbstractItemDelegateAdapter(owner);
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


if (QObjectAdapter::signalEmit(signalSignature, paramData))
  return true;

return false;
};
};
#endif // THEKLA_QABSTRACTITEMDELEGATEADAPTER_H
