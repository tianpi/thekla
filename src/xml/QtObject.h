
#ifndef THEKLA_QTOBJECT_H
#define THEKLA_QTOBJECT_H

#include "XmlElement.h"
#include "XmlElementData.h"

#include "QtSignal.h"
#include "QtProperty.h"
#include <common/Observable.h>

#include <QObject>
#include <QList>

class QtAdapter;

//--------------------------------------------------------------------------------
class THEKLA_EXPORT QtObject : public QObject, public XmlElement
{
    Q_OBJECT

public:

    typedef QList<QtSignal*> SignalList;

    typedef QList<QtProperty*> PropertyList;

private:

    static Log cLog;

private:
    QtAdapter * adapter_;

public:

    static XmlElementData getKey(const QString & className, const QString & objectName);

public:

    QtObject();

    QtObject(const QtObject & copy);

    virtual ~QtObject();

private:

    void update(QObject * qObj);

    void update(const QMetaObject * qMetaObj);

    void sync(QObject * qObj) const;

public:

    virtual XmlElement* clone() const { return new QtObject(*this); };

    virtual bool canInit(Observable::Type obsType) const;

    virtual void init(const Observable * observable);

    QObject* getQObject() const;

    QtAdapter* getAdapter();

    void setClassName(const QString & className) const
        { xmlData_->setAttribute("class-name", className); };
    QString getClassName() const
        { return xmlData_->getAttribute("class-name"); };

    void setObjectName(const QString & objectName)
        { xmlData_->setAttribute("object-name", objectName); };
    QString getObjectName() const
        { return xmlData_->getAttribute("object-name"); };

    SignalList getSignals() const;

    QtSignal * getSignal(const QString & signalSignature) const;

    PropertyList getProperties() const;

    QtProperty * getProperty(const QString & propertyName) const;

    virtual void bind();

    void bindConfiguredSignals();

    void bindConfiguredProperties();

    virtual void update();

    void updateSignal(const QString & signature, const QByteArray & text);

    void updateProperties();

    virtual void merge(const Observable * observable, bool syncObservableFirst = false);

    virtual void sync() const;

    virtual void syncObservable();

    virtual void connect(const Observer * observer);

};

#endif // THEKLA_QTOBJECT_H
