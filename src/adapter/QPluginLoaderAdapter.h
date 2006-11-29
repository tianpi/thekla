#ifndef THEKLA_QPLUGINLOADERADAPTER_H
#define THEKLA_QPLUGINLOADERADAPTER_H
#include "QObjectAdapter.h"
class QPluginLoaderAdapter : public QObjectAdapter
{
Q_OBJECT
protected:
QPluginLoaderAdapter(QtObject* owner)
 : QObjectAdapter(owner)
{};
public:
virtual ~QPluginLoaderAdapter()
 {}; 
public:
static QtAdapter* create(QtObject* owner)
{
  return new QPluginLoaderAdapter(owner);
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
#endif // THEKLA_QPLUGINLOADERADAPTER_H
