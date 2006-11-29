#ifndef THEKLA_QLIBRARYADAPTER_H
#define THEKLA_QLIBRARYADAPTER_H
#include "QObjectAdapter.h"
class QLibraryAdapter : public QObjectAdapter
{
Q_OBJECT
protected:
QLibraryAdapter(QtObject* owner)
 : QObjectAdapter(owner)
{};
public:
virtual ~QLibraryAdapter()
 {}; 
public:
static QtAdapter* create(QtObject* owner)
{
  return new QLibraryAdapter(owner);
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
#endif // THEKLA_QLIBRARYADAPTER_H
