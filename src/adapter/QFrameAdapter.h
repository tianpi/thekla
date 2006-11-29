#ifndef THEKLA_QFRAMEADAPTER_H
#define THEKLA_QFRAMEADAPTER_H
#include "QWidgetAdapter.h"
class QFrameAdapter : public QWidgetAdapter
{
Q_OBJECT
protected:
QFrameAdapter(QtObject* owner)
 : QWidgetAdapter(owner)
{};
public:
virtual ~QFrameAdapter()
 {}; 
public:
static QtAdapter* create(QtObject* owner)
{
  return new QFrameAdapter(owner);
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


if (QWidgetAdapter::signalEmit(signalSignature, paramData))
  return true;

return false;
};
};
#endif // THEKLA_QFRAMEADAPTER_H
