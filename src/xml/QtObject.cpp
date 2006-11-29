
#include "QtObject.h"
#include <common/Observable.h>
#include <common/Observer.h>
#include "XmlDocument.h"
#include "XmlDatabaseDocument.h"
#include <common/DatabaseConn.h>
#include <common/Exception.h>

#include <common/Exception.h>
#include <common/Log.h>
#include <common/QtObjectUtils.h>

#include <adapter/QtAdapterFactory.h>
#include <adapter/QtAdapter.h>

//--------------------------------------------------------------------------------
Log QtObject::cLog(Log::getClassLog("QtObject"));

//--------------------------------------------------------------------------------
QtObject::QtObject()
    : QObject(NULL),
      XmlElement(false),
      adapter_(NULL)
{
    setClassName("");
    setObjectName("");
}

//--------------------------------------------------------------------------------
QtObject::QtObject(const QtObject & copy)
    : QObject(copy.parent()),
      XmlElement(copy),
      adapter_(NULL)
{
}

//--------------------------------------------------------------------------------
QtObject::~QtObject()
{
    if (adapter_ != NULL) delete adapter_;
}

//--------------------------------------------------------------------------------
XmlElementData QtObject::getKey(const QString & className, const QString & objectName)
{
    QtObject elem;
    elem.setClassName(className);
    elem.setObjectName(objectName);
    return *(elem.xmlData_);
}

//--------------------------------------------------------------------------------
QtAdapter* QtObject::getAdapter()
{
    Log log(Log::getMethodLog(cLog, "getAdapter()"));
    THEKLA_DEBUG(log, "** START. adapter_ = " << adapter_);

    if (adapter_ != NULL) {
        THEKLA_DEBUG(log, "** END. Return existing adapter = " << adapter_);
        return adapter_;
    }

    // no adapter factory without the document
    if (ownerDoc_ == NULL) {
        THROW_THEKLA_EXCEPTION("Element does not poccess any owner Document given. "
                              << " Thus it does not have access to the QtAdapterFactory.");
    }

    // fetch adapter
    QtAdapterFactory * adapterFactory = ownerDoc_->getAdapterFactory();
    adapter_ = adapterFactory->getAdapter(getClassName(), this);
    if (adapter_ == NULL) {
        THROW_THEKLA_EXCEPTION("No QtAdapter available for given QtObject with className (" << getClassName() << ").");
    }

    THEKLA_DEBUG(log, "** END. Return new adapter = " << adapter_);
    return adapter_;
}

//--------------------------------------------------------------------------------
QObject* QtObject::getQObject() const
{
    if (observable_ == NULL)
        return NULL;
//     if (observable_->type() != Observable::QOBJECT)
//         return NULL;
    QObject * obj;
    observable_->getObservable(obj);
    return obj;
}

//--------------------------------------------------------------------------------
void QtObject::bind()
{
    //// Prepare

    XmlElement::bindBefore();

    //// Specific

    // Bind all signals
    QList<QtSignal*> qtSignalList = getSignals();
    for (int i = 0; i < qtSignalList.size(); i++) {
        QtSignal * qtSignal = qtSignalList[i];
        qtSignal->bind();
    }

    // Bind all properties
    QList<QtProperty*> qtPropertyList = getProperties();
    for (int i = 0; i < qtPropertyList.size(); i++) {
        QtProperty * qtProperty = qtPropertyList[i];
        qtProperty->bind();
    }

    //// Finalize

    XmlElement::bindAfter();

}


//--------------------------------------------------------------------------------
void QtObject::bindConfiguredSignals()
{
    Log log(Log::getMethodLog(cLog, "bindConfiguredSignals()"));

    //// Prepare

    XmlElement::bindBefore();

    //// Specific

    // get necessary objects

    QtAdapter * adapter = getAdapter();
    QObject * qObj;
    observable_->getObservable(qObj);


    // Bind Signals

    QList<QtSignal*> qtSignalList = getSignals();

    for (int i = 0; i < qtSignalList.size(); i++) {

        QtSignal * qtSignal = qtSignalList[i];
        THEKLA_DEBUG(log, "-- Signal signature = " << qtSignal->getSignature().toStdString());

        // Bind Signal -> Adapter Slot

        if (qtSignal->getXmlData().getConfigFlag(XmlElementData::BIND))
        {
            THEKLA_DEBUG(log, "BIND == true.");
            qtSignal->bindToAdapterSlot(qObj, adapter);
        }

        // Signal -> Adapter Property Update Trigger Slot

        if (qtSignal->getXmlData().getConfigFlag(XmlElementData::USE_AS_TRIGGER))
        {
            THEKLA_DEBUG(log, "USE_AS_TRIGGER == true.");
            qtSignal->bindAsTrigger(qObj, adapter);
        }

    }

    //// Finalize

    XmlElement::bindAfter();
}

//--------------------------------------------------------------------------------
void QtObject::bindConfiguredProperties()
{
    //// Prepare

    XmlElement::bindBefore();

    //// Specific

    // Bind Properties

    QList<QtProperty*> qtPropertyList = getProperties();

    for (int i = 0; i < qtPropertyList.size(); i++) {

        QtProperty * qtProperty = qtPropertyList[i];

        // Bind Property -> Adapter Slot

        if (qtProperty->getXmlData().getConfigFlag(XmlElementData::UPDATE))
        {
            qtProperty->bind();
        }
    }

    //// Finalize

    XmlElement::bindAfter();
}


//--------------------------------------------------------------------------------
void QtObject::update()
{
    QObject * qObj = getQObject();
    if (qObj == NULL)
        return;

    // store current XPath
    QString xPath = getXPath();

    // do changes
    update(qObj);
    update(qObj->metaObject());

    // XPath changed due to changes: udpate invalid, must insert
    if (xPath != getXPath()) {
        insertIntoDB();
        return;
    }
}

//--------------------------------------------------------------------------------
QtObject::SignalList QtObject::getSignals() const
{
    SignalList qtSignalList;
    for (int i = 0; i < getChildCount(); i++) {
        QtSignal * qtSignal = dynamic_cast<QtSignal*>(getChild(i));
        if (qtSignal == NULL)
            continue;
        qtSignalList.push_back(qtSignal);
    }
    return qtSignalList;
}


//--------------------------------------------------------------------------------
QtSignal * QtObject::getSignal(const QString & signalSignature) const
{
    SignalList qtSigList = getSignals();
    for (int i = 0; i < qtSigList.size(); i++) {
        QtSignal * qtSig = qtSigList[i];
        if (qtSig->getSignature() == signalSignature)
            return qtSig;
    }
    return NULL;
}

//--------------------------------------------------------------------------------
QtObject::PropertyList QtObject::getProperties() const
{
    PropertyList qtPropertyList;
    for (int i = 0; i < getChildCount(); i++) {
        QtProperty * qtProperty = dynamic_cast<QtProperty*>(getChild(i));
        if (qtProperty == NULL)
            continue;
        qtPropertyList.push_back(qtProperty);
    }
    return qtPropertyList;
}

//--------------------------------------------------------------------------------
QtProperty * QtObject::getProperty(const QString & propertyName) const
{
    PropertyList qtPropertyList = getProperties();
    for (int i = 0; i < qtPropertyList.size(); i++) {
        QtProperty * qtProperty = qtPropertyList[i];
        if (qtProperty->getName() == propertyName)
            return qtProperty;
    }
    return NULL;
}

//--------------------------------------------------------------------------------
bool QtObject::canInit(Observable::Type obsType) const
{
    switch (obsType)
    {
    case Observable::QOBJECT:
        return true;
    default:
        return false;
    }
    return false;
}

//--------------------------------------------------------------------------------
void QtObject::init(const Observable * observable)
{
    XmlElement::init(observable);

    switch (observable->type())
    {

    case Observable::QOBJECT:
        QObject * qObj;
        observable->getObservable(qObj);
        update(qObj);
        update(qObj->metaObject());
        break;

    default:
        THROW_THEKLA_EXCEPTION("Cannot initialize Element with given Observable type (" << observable->type() << ").");
    }
}

//--------------------------------------------------------------------------------
void QtObject::update(QObject * qObj)
{
    // attributes
    xmlData_->setAttribute("object-name", qObj->objectName());

    // set given QObject as parent within the Qt object tree (memory management! this
    // object is destroyed together with the parent)
    QObject::setParent(qObj);
}

//--------------------------------------------------------------------------------
void QtObject::update(const QMetaObject * qMetaObj)
{
    //// save QObject class name

    QString qObjClassName(qMetaObj->className());

    // example: rename QDesignerLabel -> QLabel
    qObjClassName = qObjClassName.remove("Designer");

    setClassName(qObjClassName);
}

//--------------------------------------------------------------------------------
void QtObject::sync(QObject * qObj) const
{
    qObj->setObjectName(getObjectName());
}

//--------------------------------------------------------------------------------
void QtObject::merge(const Observable * observable, bool syncObservableFirst)
{
    // sync the Observable w/ local XML first
    if (syncObservableFirst) {
        if (observable->type() == Observable::QOBJECT) {
            QObject * qObj;
            observable->getObservable(qObj);
            sync(qObj);
        }
    }

    // merge Observable into local XML data
    XmlElement::merge(observable, syncObservableFirst);
}

//--------------------------------------------------------------------------------
void QtObject::updateProperties()
{
    Log log(Log::getMethodLog(cLog, "updateProperties()"));
    THEKLA_DEBUG(log, "** START.");

    QObject * qObj = getQObject();
    if (qObj == NULL) {
        THEKLA_WARN(log, "** END. QObject MUST not be NULL at that point.");
        return;
    }

    //// update XML representation by reading actual Observable properties

    QList<XmlElement*> xmlElemList;
    PropertyList qtPropertyList = getProperties();

    THEKLA_DEBUG(log, "qtPropertyList.size() = " << qtPropertyList.size());

    for (int i = 0; i < qtPropertyList.size(); i++) {

        QtProperty * qtProperty = qtPropertyList[i];
        if (!qtProperty->getXmlData().getConfigFlag(XmlElementData::UPDATE))
            continue;
        qtProperty->update(qObj);
        xmlElemList.push_back(qtProperty);
    }

    //// update XML representation in DB

    updateSetDB(xmlElemList);

    THEKLA_DEBUG(log, "** END.");
}

//--------------------------------------------------------------------------------
void QtObject::updateSignal(const QString & signature, const QByteArray & text)
{
    Log log(Log::getMethodLog(cLog, "updateSignal()"));
    THEKLA_DEBUG(log, "** START. signature = " << signature.toStdString() << ", text = " << text.data());

    QtSignal * qtSignal = getSignal(signature);
    if (qtSignal == NULL) return;

    qtSignal->getXmlData().setText(text, true);

    qtSignal->update();
    THEKLA_DEBUG(log, "** END.");
}

//--------------------------------------------------------------------------------
void QtObject::syncObservable()
{
    if (observable_ == NULL)
        return;

    QObject * qObj;
    observable_->getObservable(qObj);
    sync(qObj);
}



//--------------------------------------------------------------------------------
void QtObject::connect(const Observer * observer)
{
    //// Prepare

    XmlElement::connectBefore(observer);

    //// Specific

    switch (observer->type())
    {
    case Observer::QOBJECT:
    {
        QObject * qObj;
        observer->getObserver(qObj);

        QString qObjClassName(qObj->metaObject()->className());
        if (qObjClassName != getClassName()) {
            THROW_THEKLA_EXCEPTION("Given Observer QObject (class = " << qObjClassName << ")"
                                   << "cannot be connected "
                                   << " to this element (class = " << getClassName() << ").");
        }

        // NOTE: do not know slots to connect to

//         // Connect all signals -> observer slots
//         QList<QtSignal*> qtSignalList = getSignals();
//         for (int i = 0; i < qtSignalList.size(); i++) {
//             QtSignal * qtSignal = qtSignalList[i];
//             qtSignal->connect(new Observable(qObj, QtObjectUtils::getMetaMethod(*qObj, qtSignal->getSignature())));
//         }

        // Connect all properties -> observer properties
        QList<QtProperty*> qtPropertyList = getProperties();
        for (int i = 0; i < qtPropertyList.size(); i++) {
            QtProperty * qtProperty = qtPropertyList[i];
            Observer * qPropObs = NULL;
            try {
                QMetaProperty qProp = QtObjectUtils::getMetaProperty(*qObj, qtProperty->getName().toAscii());
                qPropObs = new Observer(qObj, qProp);
                qtProperty->connect(qPropObs);
            } catch (Exception & ex) {
                if (qPropObs != NULL) delete qPropObs;
            }
        }

        break;
    }

    default:
    {
        THROW_THEKLA_EXCEPTION("Given Observer type (" << observer->type() << ") not connectable with this Element.");
    }

    }

    //// Finalize

    XmlElement::connectAfter(observer);
}

//--------------------------------------------------------------------------------
void QtObject::sync() const
{
    Log log(Log::getMethodLog(cLog, "sync()"));
    THEKLA_DEBUG(log, "** START.");

    for (int i = 0; i < observerList_.size(); i++) {

        const Observer * observer = observerList_[i];

        switch (observer->type()) {

        case Observer::QOBJECT:
        {
            QObject * qObj;
            observer->getObserver(qObj);
            sync(qObj);
            break;
        }
        default:
            break;
        };

    }

    THEKLA_DEBUG(log, "** END.");
}

