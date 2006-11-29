
#ifndef THEKLA_QTPROPERTY_H
#define THEKLA_QTPROPERTY_H

#include "XmlElement.h"
#include "XmlElementData.h"

#include <common/Observable.h>

class QMetaProperty;
class QtObject;
class Observer;

//--------------------------------------------------------------------------------
class QtProperty : public XmlElement
{
private:

    friend class QtObject;

private:

    static Log cLog;

private:

    QtObject * parentQtObject_;

public:

    static XmlElementData getKey(const QString & propertyName, const QString & propertyType);

public:

    QtProperty();

    QtProperty(const QtProperty & copy);

    virtual ~QtProperty();

private:

    void update(const QMetaProperty * qProperty);

    void update(const QObject * qObj);

    void sync(QObject * qObj, const QString & propertyName) const;

    QObject* getQObject() const;

    QMetaProperty * getQMetaProperty() const;

    void connectToProperty(QObject * qObj, QMetaProperty * qProp);

public:

    virtual XmlElement * clone() const { return new QtProperty(*this); };

    virtual bool canInit(Observable::Type obsType) const;

    virtual void init(const Observable * observable);

    virtual XmlElement* setParent(XmlElement * parent);

    QString getName() const
        { return xmlData_->getAttribute("name"); };
    QString getType() const
        { return xmlData_->getAttribute("type"); };

    virtual void update();

    virtual void merge(const Observable * observable, bool syncObservableFirst = false);

    virtual void connect(const Observer * observer);

    virtual void sync() const;

    virtual void syncObservable();

    virtual void bind();

};

#endif // THEKLA_QTPROPERTY_H
