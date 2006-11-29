#ifndef THEKLA_QTRANSLATORADAPTER_H
#define THEKLA_QTRANSLATORADAPTER_H
#include "QObjectAdapter.h"
class QTranslatorAdapter : public QObjectAdapter
{
Q_OBJECT
protected:
QTranslatorAdapter(QtObject* owner)
 : QObjectAdapter(owner)
{};
public:
virtual ~QTranslatorAdapter()
 {}; 
public:
static QtAdapter* create(QtObject* owner)
{
  return new QTranslatorAdapter(owner);
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
#endif // THEKLA_QTRANSLATORADAPTER_H
