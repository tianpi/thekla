#ifndef THEKLA_QMESSAGEBOXADAPTER_H
#define THEKLA_QMESSAGEBOXADAPTER_H
#include "QDialogAdapter.h"
class QMessageBoxAdapter : public QDialogAdapter
{
Q_OBJECT
protected:
QMessageBoxAdapter(QtObject* owner)
 : QDialogAdapter(owner)
{};
public:
virtual ~QMessageBoxAdapter()
 {}; 
public:
static QtAdapter* create(QtObject* owner)
{
  return new QMessageBoxAdapter(owner);
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
#endif // THEKLA_QMESSAGEBOXADAPTER_H
