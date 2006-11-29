#ifndef THEKLA_QWORKSPACEADAPTER_H
#define THEKLA_QWORKSPACEADAPTER_H
#include "QWidgetAdapter.h"
#include <QWidget>
class QWorkspaceAdapter : public QWidgetAdapter
{
Q_OBJECT
protected:
QWorkspaceAdapter(QtObject* owner)
 : QWidgetAdapter(owner)
{};
public:
virtual ~QWorkspaceAdapter()
 {}; 
public:
static QtAdapter* create(QtObject* owner)
{
  return new QWorkspaceAdapter(owner);
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
#endif // THEKLA_QWORKSPACEADAPTER_H
