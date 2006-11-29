
#include "OivField.h"
#include <common/Exception.h>
#include <common/Observer.h>
#include <common/TypeConverter.h>
#include <common/Log.h>

#include <Inventor/fields/SoFields.h>
#include <Inventor/fields/SoField.h>
#include <Inventor/SoType.h>
#include <Inventor/SbName.h>
#include <Inventor/SbString.h>

//--------------------------------------------------------------------------------
Log OivField::cLog(Log::getClassLog("OivField", log4cplus::OFF_LOG_LEVEL));

//--------------------------------------------------------------------------------
XmlElementData OivField::getKey(const QString & type, const QString & name)
{
    OivField ret;
    ret.getXmlData().setAttribute("type", type);
    ret.getXmlData().setAttribute("name", name);
    return ret.getXmlData();
}

//--------------------------------------------------------------------------------
OivField::OivField()
    : XmlElement(true),
      fieldSensor_(NULL),
      adapter_(NULL)
{
    xmlData_->setAttribute("type", "");
    xmlData_->setAttribute("name", "");
}

//--------------------------------------------------------------------------------
OivField::OivField(const OivField & copy)
    : XmlElement(copy),
      fieldSensor_(copy.fieldSensor_),
      adapter_(copy.adapter_)
{
}

//--------------------------------------------------------------------------------
OivField::~OivField()
{
    if (fieldSensor_ != NULL) delete fieldSensor_;
    if (adapter_ != NULL) delete adapter_;
}

//--------------------------------------------------------------------------------
bool OivField::canInit(Observable::Type obsType) const
{
    switch (obsType) {

    case Observable::SOFIELD:
        return true;
    default:
        return false;
    }

    return false;
}

//--------------------------------------------------------------------------------
void OivField::init(const Observable * observable)
{
    XmlElement::init(observable);

    switch (observable->type()) {

    case Observable::SOFIELD:

        SoField * field;
        observable->getObservable(field);

        update(field);

        break;

    default:
        THROW_THEKLA_EXCEPTION("Element cannot be initialized with given Observable type (" << observable->type() << ").");
    }

}

//--------------------------------------------------------------------------------
void OivField::update(SoField * field)
{
    Log log(Log::getMethodLog(cLog, "update(SoField)"));
    THEKLA_DEBUG(log, "** START. getText() = " << xmlData_->getText().data());

    xmlData_->setAttribute("type", field->getTypeId().getName().getString());

    SbString fieldText;
    field->get(fieldText);

    THEKLA_DEBUG(log, "fieldText.getString() = " << fieldText.getString()
                << ", fieldText.getLength() = " << fieldText.getLength());

    QByteArray text(fieldText.getString(), fieldText.getLength());
    xmlData_->setText(text);

    THEKLA_DEBUG(log, "** END. getText() = " << xmlData_->getText().data());
}

//--------------------------------------------------------------------------------
void OivField::update()
{
    Log log(Log::getMethodLog(cLog, "update()"));
    THEKLA_DEBUG(log, "** START.");

    if (observable_ == NULL)
        return;

    // update this element with actual Observable values
    SoField * field;
    observable_->getObservable(field);
    update(field);

    // update database entry
    XmlElement::update();

    THEKLA_DEBUG(log, "** END.");
}

//--------------------------------------------------------------------------------
void OivField::merge(const Observable * observable, bool syncObservableFirst )
{
    // sync the Observable w/ local XML first
    if (syncObservableFirst) {
        if (observable->type() == Observable::SOFIELD) {
            SoField * field;
            observable->getObservable(field);
            sync(field);
        }
    }

    // merge Observable into local XML data
    XmlElement::merge(observable, syncObservableFirst);
}

//--------------------------------------------------------------------------------
void OivField::sync(SoField * field) const
{
    field->set(xmlData_->getText().data());
}

//--------------------------------------------------------------------------------
void OivField::bind()
{
    //// Prepare

    XmlElement::bindBefore();

    //// Specific

    // Already bound to Observable: Return silently
    if (fieldSensor_ != NULL)
        return;

    SoField * field;
    observable_->getObservable(field);

    // attach the Observable to the FieldSensor
    fieldSensor_ = new SoFieldSensor(OivField::fieldSensorCB, this);
    fieldSensor_->setDeleteCallback(OivField::fieldSensorDeleteCB, this);
    fieldSensor_->attach(field);
//    fieldSensor_->schedule(); // optional ??

    fieldSensor_->trigger();

    //// Finalize

    XmlElement::bindAfter();
}

//--------------------------------------------------------------------------------
void OivField::fieldSensorCB(void * data, SoSensor * sensor)
{
    Log log(Log::getMethodLog(cLog, "fieldSensorCB()"));
    THEKLA_DEBUG(log, "** START.");

    Q_UNUSED(sensor);

    OivField * oivField = reinterpret_cast<OivField*>(data);

    // Update the XmlElement (object and database)
    oivField->update();

    THEKLA_DEBUG(log, "** END.");
}

//--------------------------------------------------------------------------------
/**
 * If an object monitored by a data sensor is deleted, the given callback function will be
 * called with the given userdata
 */
void OivField::fieldSensorDeleteCB(void * data, SoSensor * sensor)
{
    Log log(Log::getMethodLog(cLog, "fieldSensorDeleteCB()", log4cplus::ALL_LOG_LEVEL));
    THEKLA_DEBUG(log, "** START.");

    Q_UNUSED(data);
    Q_UNUSED(sensor);

//    OivField * oivField = reinterpret_cast<OivField*>(data);
//    delete oivField;

    THEKLA_DEBUG(log, "** END.");
}


//--------------------------------------------------------------------------------
void OivField::connect(const Observer * observer)
{
    //// Prepare

    XmlElement::connectBefore(observer);

    //// Specific

    switch (observer->type())
    {
    case Observer::QOBJECTPROPERTY:
    case Observer::SOFIELD:
        if (!observer->isConnectable(TypeConverter::getTypeFromName(getType())))
            THROW_THEKLA_EXCEPTION("Given Observer object value types are not compatible with this Element.");
        break;
    default:
        THROW_THEKLA_EXCEPTION("Given Observer type (" << observer->type() << ") is not supported by this Element");
    }

    // Create Adapter (SoField Adapter -> SoField Observer)
    if (adapter_ == NULL) {
        adapter_ = TypeConverter::getInstance(TypeConverter::getTypeFromName(getType()));
        if (adapter_ == NULL) {
            THROW_THEKLA_EXCEPTION("No SoField Adapter available for Element type (" << getType() << ").");
        }
    }

    //// Finalize

    XmlElement::connectAfter(observer);
}


//--------------------------------------------------------------------------------
void OivField::sync() const
{
    //// Update SoField adapter

    adapter_->set(xmlData_->getText());


    //// Update Observers

    for (int i = 0; i < observerList_.size(); i++) {

        const Observer * observer = observerList_[i];

        switch (observer->type()) {

        case Observer::QOBJECTPROPERTY:
        {
            QObject * qObj;
            QMetaProperty * qProperty;
            observer->getObserver(qObj, qProperty);

            // convert SoField -> QVariant
            QVariant propertyValue;

            // Anything (except String) -> String
            if (qProperty->type() == QVariant::String &&
                adapter_->getTypeId() != TypeConverter::findType(QVariant::String))
            {
                propertyValue.setValue(xmlData_->getText());
            }
            // Field Type -> equivalent QString
            else
            {
                TypeConverter::convert(*adapter_, propertyValue);
            }

            // set QObject property
            qObj->setProperty(qProperty->name(), propertyValue);

            break;
        }
        case Observer::SOFIELD:
        {
            SoField * field;
            observer->getObserver(field);

            // set Observer SoField text
            field->set(xmlData_->getText().data());

            break;
        }

        default:
            THROW_THEKLA_EXCEPTION("Given Observer type (" << observer->type() << ") is not supported.")

        };
    }

}

//--------------------------------------------------------------------------------
void OivField::syncObservable()
{
    if (observable_ == NULL)
        return;

    SoField * field;
    observable_->getObservable(field);
    sync(field);
}

