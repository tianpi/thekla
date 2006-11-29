
#include "QtSignal.h"

#include "XmlElementData.h"
#include <common/Observable.h>
#include "XmlDocument.h"
#include "QtObject.h"
#include <common/QtObjectUtils.h>
#include <common/QtAdapterUtils.h>
#include <common/Observer.h>
#include <common/Exception.h>
#include <common/TypeConverter.h>
#include <common/Log.h>
#include <adapter/QtAdapter.h>

//--------------------------------------------------------------------------------
Log QtSignal::cLog(Log::getClassLog("QtSignal"));

//--------------------------------------------------------------------------------
QtSignal::QtSignal()
    : XmlElement(true),
      parentQtObject_(NULL)
{
    xmlData_->setAttribute("signature", "");
}

//--------------------------------------------------------------------------------
QtSignal::QtSignal(const QtSignal & copy)
    : XmlElement(copy),
      parentQtObject_(NULL)
{
}

//--------------------------------------------------------------------------------
QtSignal::~QtSignal()
{
}

//--------------------------------------------------------------------------------
XmlElementData QtSignal::getKey(const QString & signature)
{
    QtSignal elem;
    elem.xmlData_->setAttribute("signature", signature);
    return *(elem.xmlData_);
}


//--------------------------------------------------------------------------------
QString QtSignal::getSignature() const
{
    return xmlData_->getAttribute("signature");
}

//--------------------------------------------------------------------------------
bool QtSignal::canInit(Observable::Type obsType) const
{
    switch (obsType)
    {
    case Observable::QOBJECTSIGNAL:
        return true;
    default:
        return false;
    }
    return false;
}

//--------------------------------------------------------------------------------
void QtSignal::init(const Observable * observable)
{
    XmlElement::init(observable);

    switch (observable->type())
    {

    case Observable::QOBJECTSIGNAL:
        QObject * qObj;
        QMetaMethod * qSignal;
        observable->getObservable(qObj, qSignal);

        if (qSignal->methodType() != QMetaMethod::Signal) {
            THROW_THEKLA_EXCEPTION("Given Observable QMetaMethod "
                                  << "(signature = " << QString(qSignal->signature()) << ") "
                                  << "is NOT a Signal.");
        }

        update(qSignal);
        break;

    default:
        THROW_THEKLA_EXCEPTION("Given Observable type (" << observable->type() << ") not supported.");

    }
}

//--------------------------------------------------------------------------------
void QtSignal::update(QMetaMethod * qSignal)
{
    Log log(Log::getMethodLog(cLog, "update(QMetaMethod)"));
    THEKLA_DEBUG(log, "** START. getSignature() = " << getSignature().toStdString());

    xmlData_->setAttribute("signature", qSignal->signature());

    THEKLA_DEBUG(log, "** END. getSignature() = " << getSignature().toStdString());
}

//--------------------------------------------------------------------------------
void QtSignal::merge(const Observable * observable, bool syncObservableFirst)
{
    // sync the Observable w/ local XML first
    if (syncObservableFirst) {
        // nothing to do
    }

    // merge Observable into local XML data
    XmlElement::merge(observable, syncObservableFirst);
}


//--------------------------------------------------------------------------------
XmlElement* QtSignal::setParent(XmlElement * parent)
{
	XmlElement * prev = XmlElement::setParent(parent);
    parentQtObject_ = dynamic_cast<QtObject*>(parent);
	return prev;
}


//--------------------------------------------------------------------------------
void QtSignal::bindToAdapterSlot(QObject * observable, QtAdapter * adapter)
{
    Log log(Log::getMethodLog(cLog, "bindToAdapterSlot()"));
    THEKLA_DEBUG(log, "** START.");

    QString signalSig = getSignature();
    QString slotSig = QtAdapterUtils::getSlotSignature(getSignature());

    THEKLA_DEBUG(log, "Connect (Object) Signal " << signalSig.toStdString()
                << " -> (Adapter) Slot " << signalSig.toStdString());

    if (!QObject::connect(observable, QtObjectUtils::addSignalPrefix(signalSig),
                          (QObject*)adapter, QtObjectUtils::addSlotPrefix(slotSig)))
    {
        THROW_THEKLA_EXCEPTION("Connecting Signal (" << signalSig << ") with Slot (" << slotSig << ") failed.");
    }

    THEKLA_DEBUG(log, "** END.");
}

//--------------------------------------------------------------------------------
void QtSignal::bindAsTrigger(QObject * observable, QtAdapter * adapter)
{
    Log log(Log::getMethodLog(cLog, "bindAsTrigger()"));
    THEKLA_DEBUG(log, "** START.");

    QString signalSig = getSignature();
    QString slotSig = "propertyUpdateTrigger()";

    THEKLA_DEBUG(log, "connect (Object) Signal " << signalSig.toStdString()
                << " -> (Adapter) Property trigger Slot " << slotSig.toStdString());

    if (!QObject::connect(observable, QtObjectUtils::addSignalPrefix(signalSig),
                          (QObject*)adapter, QtObjectUtils::addSlotPrefix(slotSig)))
    {
        THROW_THEKLA_EXCEPTION("Connecting Signal (" << signalSig << ") with Slot (" << slotSig << ") failed.");
    }

    THEKLA_DEBUG(log, "** END.");
}

//--------------------------------------------------------------------------------
void QtSignal::bind()
{
    Log log(Log::getMethodLog(cLog, "bind()"));
    THEKLA_DEBUG(log, "** START.");

    //// Prepare

    XmlElement::bindBefore();

    //// Specific


    //// need Adapter from QtObject base class

    if (parentQtObject_ == NULL) {
        THROW_THEKLA_EXCEPTION("This Element does not have a QtObject parent.");
    }
    QtAdapter * adapter = parentQtObject_->getAdapter();
    if (adapter == NULL) {
        THROW_THEKLA_EXCEPTION("Parent QtObject returned invalid QtAdapter (NULL pointer).");
    }

    //// Connect QObject Signal -> Adapter Slot

    QObject * qObj;
    QMetaMethod * qSignal;
    observable_->getObservable(qObj, qSignal);

    bindToAdapterSlot(qObj, adapter);
    bindAsTrigger(qObj, adapter);

    //// Finalize

    XmlElement::bindAfter();

    THEKLA_DEBUG(log, "** END.");
}

//--------------------------------------------------------------------------------
void QtSignal::connect(const Observer * observer)
{
    //// Prepare

    XmlElement::connectBefore(observer);

    //// Specific

    switch (observer->type())
    {
    case Observer::QOBJECTSLOT:
    {
        QObject * qObj;
        QMetaMethod * qSlot;
        observer->getObserver(qObj, qSlot);

        if (qSlot->methodType() != QMetaMethod::Slot) {
            THROW_THEKLA_EXCEPTION("Given Observer QMetaMethod is not a Slot.");
        }
        if (parentQtObject_ == NULL) {
            THROW_THEKLA_EXCEPTION("Element does not have a QtObject parent.");
        }
        if (parentQtObject_->getAdapter() == NULL) {
            THROW_THEKLA_EXCEPTION("QtObject returned an invalid QtAdapter (NULL pointer).");
        }

        connectToSlot(qObj, qSlot);

        break;
    }

    case Observer::SOFIELDLIST:
    {
        // Observer
        SoFieldList * fieldList;
        observer->getObserver(fieldList);

        // Signal Parameter Types (String)
        QStringList strTypeList = QtObjectUtils::getParamTypes(getSignature());

        // Signal Parameter Types (QVariant)
        QList<QVariant::Type> qVarTypeList;

        for (int i = 0; i < strTypeList.size(); i++) {

            QString strType = QtObjectUtils::normalizeParamType(strTypeList[i]);
            QVariant qVar = QVariant::nameToType(strType.toAscii());
            qVarTypeList.push_back(qVar.type());
        }

        if (!observer->isConnectable(qVarTypeList))
            THROW_THEKLA_EXCEPTION("Given Observer object type(s) are not compatible with this Element.");

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
void QtSignal::connectToSlot(QObject * qObj, QMetaMethod * qSlot)
{
    Log log(Log::getMethodLog(cLog, "connectToSlot()"));

    THEKLA_DEBUG(log, "** START.");

    // create, retrieve an adapter
    QtAdapter * adapter = parentQtObject_->getAdapter();

    // Connect Adapter Signal with Observer slot

    QString signalSig = QtAdapterUtils::getSignalSignature(getSignature());
    QString slotSig = qSlot->signature();

    if (!QObject::connect((QObject*)adapter, QtObjectUtils::addSignalPrefix(signalSig),
                          qObj, QtObjectUtils::addSlotPrefix(slotSig)))
    {
        THROW_THEKLA_EXCEPTION("Connecting Adapter signal (" << signalSig << ") with Observer slot (" << slotSig << ") failed.");
    }

    THEKLA_DEBUG(log, "** END");
}

//--------------------------------------------------------------------------------
void QtSignal::update()
{
    XmlElement::update();
}

//--------------------------------------------------------------------------------
void QtSignal::sync() const
{
    Log log(Log::getMethodLog(cLog, "sync()"));
    THEKLA_DEBUG(log, "** START.");

    for (int i = 0; i < observerList_.size(); i++) {

        const Observer * observer = observerList_[i];

        switch (observer->type()) {

        case Observer::QOBJECTSLOT:
        {
            if (parentQtObject_ == NULL)
                break;
            QtAdapter * adapter = parentQtObject_->getAdapter();

            bool ret = adapter->signalEmit(getSignature(), xmlData_->getText(true));
            THEKLA_DEBUG(log, "QOBJECTSLOT, signalEmit() = " << ret);
            break;
        }
        case Observer::SOFIELDLIST:
        {
            SoFieldList * fieldList;
            observer->getObserver(fieldList);

            QList<QVariant> valueList = QtObjectUtils::fromStream(xmlData_->getText(true));

            // Signal with NO parameters -> SoSFTrigger
            if (valueList.size() == 0) {

                SoField * field = fieldList->get(0);
                QVariant v(QVariant::Invalid);
                TypeConverter::convert(v, *field);

            }
            // Signal with n parameters: Convert QVariant -> SoField
            else {

                for (int i = 0; i < fieldList->getLength(); i++) {
                    SoField * field = fieldList->get(i);
                    TypeConverter::convert(valueList[i], *field);
                }

            }

            break;
        }

        default:
            break;

        };

    }

    THEKLA_DEBUG(log, "** END.");

}

