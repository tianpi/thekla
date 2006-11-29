#ifndef THEKLA_QSETTINGSADAPTER_H
#define THEKLA_QSETTINGSADAPTER_H
#include "QObjectAdapter.h"
class QSettingsAdapter : public QObjectAdapter
{
Q_OBJECT
protected:
QSettingsAdapter(QtObject* owner)
 : QObjectAdapter(owner)
{};
public:
virtual ~QSettingsAdapter()
 {}; 
public:
static QtAdapter* create(QtObject* owner)
{
  return new QSettingsAdapter(owner);
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
#endif // THEKLA_QSETTINGSADAPTER_H
