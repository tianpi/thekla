#ifndef THEKLA_QDATEEDITADAPTER_H
#define THEKLA_QDATEEDITADAPTER_H
#include "QDateTimeEditAdapter.h"
class QDateEditAdapter : public QDateTimeEditAdapter
{
Q_OBJECT
protected:
QDateEditAdapter(QtObject* owner)
 : QDateTimeEditAdapter(owner)
{};
public:
virtual ~QDateEditAdapter()
 {}; 
public:
static QtAdapter* create(QtObject* owner)
{
  return new QDateEditAdapter(owner);
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


if (QDateTimeEditAdapter::signalEmit(signalSignature, paramData))
  return true;

return false;
};
};
#endif // THEKLA_QDATEEDITADAPTER_H
