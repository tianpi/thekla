#ifndef THEKLA_QERRORMESSAGEADAPTER_H
#define THEKLA_QERRORMESSAGEADAPTER_H
#include "QDialogAdapter.h"
class QErrorMessageAdapter : public QDialogAdapter
{
Q_OBJECT
protected:
QErrorMessageAdapter(QtObject* owner)
 : QDialogAdapter(owner)
{};
public:
virtual ~QErrorMessageAdapter()
 {}; 
public:
static QtAdapter* create(QtObject* owner)
{
  return new QErrorMessageAdapter(owner);
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


if (QDialogAdapter::signalEmit(signalSignature, paramData))
  return true;

return false;
};
};
#endif // THEKLA_QERRORMESSAGEADAPTER_H
