#ifndef THEKLA_QTIMEEDITADAPTER_H
#define THEKLA_QTIMEEDITADAPTER_H
#include "QDateTimeEditAdapter.h"
class QTimeEditAdapter : public QDateTimeEditAdapter
{
Q_OBJECT
protected:
QTimeEditAdapter(QtObject* owner)
 : QDateTimeEditAdapter(owner)
{};
public:
virtual ~QTimeEditAdapter()
 {}; 
public:
static QtAdapter* create(QtObject* owner)
{
  return new QTimeEditAdapter(owner);
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
#endif // THEKLA_QTIMEEDITADAPTER_H
