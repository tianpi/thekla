
#include "QtProperty.h"
#include <common/Observable.h>
#include "QtObject.h"
#include <common/QtObjectUtils.h>
#include <common/Observer.h>
#include <common/Exception.h>
#include <common/TypeConverter.h>
#include <common/Log.h>

//--------------------------------------------------------------------------------
Log QtProperty::cLog(Log::getClassLog("QtProperty"));

//--------------------------------------------------------------------------------
QtProperty::QtProperty()
    : XmlElement(true),
      parentQtObject_(NULL)
{
    xmlData_->setAttribute("name", "");
    xmlData_->setAttribute("type", "");
}

//--------------------------------------------------------------------------------
QtProperty::QtProperty(const QtProperty & copy)
    : XmlElement(copy),
      parentQtObject_(NULL)
{
}

//--------------------------------------------------------------------------------
QtProperty::~QtProperty()
{

}

//--------------------------------------------------------------------------------
XmlElementData QtProperty::getKey(const QString & propertyName, const QString & propertyType)
{
    QtProperty elem;
    elem.getXmlData().setAttribute("name", propertyName);
    elem.getXmlData().setAttribute("type", propertyType);
    return elem.getXmlData();
}


//--------------------------------------------------------------------------------
XmlElement* QtProperty::setParent(XmlElement * parent)
{
    XmlElement * prev = XmlElement::setParent(parent);
    parentQtObject_ = dynamic_cast<QtObject*>(parent);
	return prev;
}

//--------------------------------------------------------------------------------
bool QtProperty::canInit(Observable::Type obsType) const
{
    switch (obsType)
    {
    case Observable::QOBJECTPROPERTY:
        return true;
    default:
        return false;
    }
    return false;
}

//--------------------------------------------------------------------------------
void QtProperty::init(const Observable * observable)
{
    XmlElement::init(observable);

    switch (observable->type())
    {

    case Observable::QOBJECTPROPERTY:
    {
        QObject * qObj;
        QMetaProperty * qProperty;
        observable->getObservable(qObj, qProperty);
        update(qProperty);
        update(qObj);
        break;
    }
    default:
        THROW_THEKLA_EXCEPTION("Element cannot be initialized with given Observable type (" << observable->type() << ").");
    }
}

//--------------------------------------------------------------------------------
void QtProperty::merge(const Observable * observable, bool syncObservableFirst)
{
    // sync the Observable w/ local XML data first
    if (syncObservableFirst) {

        if (observable->type() == Observable::QOBJECTPROPERTY) {
            QObject * qObj;
            QMetaProperty * qProp;
            observable->getObservable(qObj, qProp);
            sync(qObj, qProp->name());
        }
    }

    // merge Observable into local XML data
    XmlElement::merge(observable, syncObservableFirst);
}

//--------------------------------------------------------------------------------
void QtProperty::update(const QMetaProperty * qProperty)
{
    xmlData_->setAttribute("name", qProperty->name());
    xmlData_->setAttribute("type", qProperty->typeName());
}

//--------------------------------------------------------------------------------
void QtProperty::update(const QObject * qObj)
{
    Log log(Log::getMethodLog(cLog, "update(QObject)", log4cplus::OFF_LOG_LEVEL));
    THEKLA_DEBUG(log, "** START.");

    // get the property value
    QVariant value = qObj->property(getName().toAscii());
    THEKLA_DEBUG(log, "qVariant.toString() = " << value.toString().toStdString());

    // convert to byte array and store as text (encode base64)
    QByteArray text = QtObjectUtils::toStream(value);
    THEKLA_DEBUG(log, "toStream() = " << text.data());
    xmlData_->setText(text, true);

    THEKLA_DEBUG(log, "** END. getText() = " << xmlData_->getText().data());
}


//--------------------------------------------------------------------------------
void QtProperty::sync(QObject * qObj, const QString & propertyName) const
{
    QVariant value = QtObjectUtils::fromStream(xmlData_->getText(true))[0];
    qObj->setProperty(propertyName.toAscii(), value);
}


//--------------------------------------------------------------------------------
QObject* QtProperty::getQObject() const
{
    if (observable_ == NULL)
        return NULL;
    QObject * qObj;
    QMetaProperty * qProp;
    observable_->getObservable(qObj, qProp);
    return qObj;
}

//--------------------------------------------------------------------------------
QMetaProperty * QtProperty::getQMetaProperty() const
{
    if (observable_ == NULL)
        return NULL;
    QObject * qObj;
    QMetaProperty * qProp;
    observable_->getObservable(qObj, qProp);
    return qProp;
}



//--------------------------------------------------------------------------------
void QtProperty::update()
{
    Log log(Log::getMethodLog(cLog, "update()", log4cplus::OFF_LOG_LEVEL));
    THEKLA_DEBUG(log, "** START. propertyName = " << getName().toStdString());

    // retrieve QObject from parent element
    QObject * qObj = getQObject();
    if (qObj == NULL) {
        THEKLA_DEBUG(log, "** END. QObject == NULL.");
        return;
    }

    // update property value
    update(qObj);

    XmlElement::update();

    THEKLA_DEBUG(log, "** END.");
}

//--------------------------------------------------------------------------------
void QtProperty::connect(const Observer * observer)
{
    Log log(Log::getMethodLog(cLog, "connect()"));
    THEKLA_DEBUG(log, "** START.");

    THEKLA_DEBUG(log, "name = " << getName().toStdString());
    THEKLA_DEBUG(log, "type = " << getType().toStdString());


    //// Prepare

    XmlElement::connectBefore(observer);

    //// Specific

    switch (observer->type())
    {
    case Observer::QOBJECTPROPERTY:
    case Observer::SOFIELD:
        if (!observer->isConnectable(QVariant::nameToType(getType().toAscii()))) {
            THROW_THEKLA_EXCEPTION("Given Observer object type is not compatible with this Element.");
        }
        break;

    default:
        THROW_THEKLA_EXCEPTION("Given Observer type (" << observer->type() << ") is not supported.")
    };

    //// Finalize

    XmlElement::connectAfter(observer);

    THEKLA_DEBUG(log, "** END.");
}

//--------------------------------------------------------------------------------
void QtProperty::sync() const
{
    Log log(Log::getMethodLog(cLog, "sync()"));
    THEKLA_DEBUG(log, "** START.");

    for (int i = 0; i < observerList_.size(); i++) {

        const Observer * observer = observerList_[i];

        switch (observer->type()) {

        case Observer::QOBJECTPROPERTY:
        {
            QObject * qObj;
            QMetaProperty * qProp;
            observer->getObserver(qObj, qProp);

            sync(qObj, qProp->name());

            THEKLA_DEBUG(log, "QOBJECTPROPERTY: name = " << qProp->name());

            break;
        }
        case Observer::SOFIELD:
        {
            SoField * field;
            observer->getObserver(field);

            QVariant value = QtObjectUtils::fromStream(xmlData_->getText(true))[0];
            TypeConverter::convert(value, *field);
            break;
        }
        default:
            break;
        };

    }

    THEKLA_DEBUG(log, "** END.");
}

//--------------------------------------------------------------------------------
void QtProperty::syncObservable()
{
    if (observable_ == NULL)
        return;

    QObject * qObj;
    QMetaProperty * qProp;
    observable_->getObservable(qObj, qProp);
    sync(qObj, qProp->name());
}

//--------------------------------------------------------------------------------
void QtProperty::bind()
{
    //// Prepare

    XmlElement::bindBefore();

    //// Specific

    xmlData_->setConfigFlag(XmlElementData::UPDATE, true);

    //// Finalize

    XmlElement::bindAfter();
}

