
#ifndef THEKLA_QTADAPTER_H
#define THEKLA_QTADAPTER_H

#include <QObject>
#include <QByteArray>
#include <QDataStream>
#include <QIODevice>
#include <QString>
#include <QVariant>

class QtObject;
class QtSignal;
class Log;

//--------------------------------------------------------------------------------
class QtAdapter : public QObject
{
    Q_OBJECT

public:

    typedef QtAdapter* (*CreatorFunc) (QtObject * owner);

public:

    static Log cLog;

    static QtAdapter* create(QtObject * owner);

protected:

    QtObject * owner_;

protected:

    QtAdapter(QtObject * owner, QObject * parent = NULL)
        : QObject(parent),
          owner_(owner)
        {};

public:

    virtual ~QtAdapter()
        {};

public:

    virtual bool signalEmit(const QString & signalSignature, const QByteArray & paramData);

protected:

    void signalChanged(const QString & signalSignature, const QByteArray & paramData);

protected slots:

    void propertyUpdateTrigger();
};

#endif // THEKLA_QTADAPTER_H
