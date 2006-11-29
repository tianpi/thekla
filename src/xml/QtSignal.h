
#ifndef THEKLA_QTSIGNAL_H
#define THEKLA_QTSIGNAL_H

#include "XmlElement.h"
#include <common/Observable.h>

#include <QList>

class QMetaMethod;
class QtObject;
class QtAdapter;
class Log;

//--------------------------------------------------------------------------------
class THEKLA_EXPORT QtSignal : public XmlElement
{
private:

    friend class QtObject;

private:

    static Log cLog;

private:

    QtObject * parentQtObject_;

public:

    static XmlElementData getKey(const QString & signature);

public:

    QtSignal();

    QtSignal(const QtSignal & copy);

    virtual ~QtSignal();

private:

    void bindAsTrigger(QObject * observable, QtAdapter * adapter);

    void bindToAdapterSlot(QObject * observable, QtAdapter * adapter);

    void update(QMetaMethod * qSignal);

    void connectToSlot(QObject * qObj, QMetaMethod * qSlot);

public:

    virtual XmlElement * clone() const { return new QtSignal(*this); };

    virtual bool canInit(Observable::Type obsType) const;

    virtual void init(const Observable * observable);

    virtual XmlElement* setParent(XmlElement * parent);

    QString getSignature() const;

    virtual void bind();

    void bindAsTrigger();

    virtual void connect(const Observer * observer);

    virtual void update();

    virtual void merge(const Observable * observable, bool syncObservableFirst = false);

    virtual void sync() const;
};

#endif // THEKLA_QTSIGNAL_H
