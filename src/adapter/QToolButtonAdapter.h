#ifndef THEKLA_QTOOLBUTTONADAPTER_H
#define THEKLA_QTOOLBUTTONADAPTER_H
#include "QAbstractButtonAdapter.h"
#include <QAction>
class QToolButtonAdapter : public QAbstractButtonAdapter
{
Q_OBJECT
protected:
QToolButtonAdapter(QtObject* owner)
 : QAbstractButtonAdapter(owner)
{};
public:
virtual ~QToolButtonAdapter()
 {}; 
public:
static QtAdapter* create(QtObject* owner)
{
  return new QToolButtonAdapter(owner);
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


if (QAbstractButtonAdapter::signalEmit(signalSignature, paramData))
  return true;

return false;
};
};
#endif // THEKLA_QTOOLBUTTONADAPTER_H
