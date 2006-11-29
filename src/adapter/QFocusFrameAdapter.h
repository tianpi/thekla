#ifndef THEKLA_QFOCUSFRAMEADAPTER_H
#define THEKLA_QFOCUSFRAMEADAPTER_H
#include "QWidgetAdapter.h"
class QFocusFrameAdapter : public QWidgetAdapter
{
Q_OBJECT
protected:
QFocusFrameAdapter(QtObject* owner)
 : QWidgetAdapter(owner)
{};
public:
virtual ~QFocusFrameAdapter()
 {}; 
public:
static QtAdapter* create(QtObject* owner)
{
  return new QFocusFrameAdapter(owner);
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
#endif // THEKLA_QFOCUSFRAMEADAPTER_H
