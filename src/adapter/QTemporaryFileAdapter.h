#ifndef THEKLA_QTEMPORARYFILEADAPTER_H
#define THEKLA_QTEMPORARYFILEADAPTER_H
#include "QFileAdapter.h"
class QTemporaryFileAdapter : public QFileAdapter
{
Q_OBJECT
protected:
QTemporaryFileAdapter(QtObject* owner)
 : QFileAdapter(owner)
{};
public:
virtual ~QTemporaryFileAdapter()
 {}; 
public:
static QtAdapter* create(QtObject* owner)
{
  return new QTemporaryFileAdapter(owner);
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


if (QFileAdapter::signalEmit(signalSignature, paramData))
  return true;

return false;
};
};
#endif // THEKLA_QTEMPORARYFILEADAPTER_H
