
#ifndef THEKLA_OIVFIELD_H
#define THEKLA_OIVFIELD_H

#include "XmlElement.h"
#include "XmlElementData.h"
#include <common/Observable.h>

#include <Inventor/fields/SoField.h>
#include <Inventor/sensors/SoFieldSensor.h>

class Observer;

//--------------------------------------------------------------------------------
class THEKLA_EXPORT OivField : public XmlElement
{
private:

    static Log cLog;

private:

    static void fieldSensorCB(void * data, SoSensor * sensor);

    static void fieldSensorDeleteCB(void * data, SoSensor * sensor);

public:

    static XmlElementData getKey(const QString & type, const QString & name);

private:

    SoFieldSensor * fieldSensor_;

    SoField * adapter_;

public:

    OivField();

    OivField(const OivField & copy);

    virtual ~OivField();

private:

    void update(SoField * field);

    void sync(QObject * qObj, const QString & propertyName) const;

    void sync(SoField * field) const;

public:

    virtual XmlElement * clone() const { return new OivField(*this); };

    virtual bool canInit(Observable::Type obsType) const;

    virtual void init(const Observable * observable);

    void setName(const QString & fieldName) const
        { return xmlData_->setAttribute("name", fieldName); };
    QString getName() const
        { return xmlData_->getAttribute("name"); };
    QString getType() const
        { return xmlData_->getAttribute("type"); };

    SoFieldSensor* getFieldSensor() const
        { return fieldSensor_; };

    virtual void update();

    virtual void merge(const Observable * observable, bool syncObservableFirst = false);

    virtual void bind();

    virtual void connect(const Observer * observer);

    virtual void sync() const;

    virtual void syncObservable();

};

#endif // THEKLA_OIVFIELD_H
