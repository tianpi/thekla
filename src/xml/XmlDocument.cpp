
#include "XmlDocument.h"
#include "XmlElement.h"
#include "XmlElementData.h"
#include "XmlElementInfo.h"
#include "QtObject.h"

#include <common/Exception.h>
#include <common/Log.h>
#include <adapter/QtAdapterFactory.h>

//--------------------------------------------------------------------------------
Log XmlDocument::cLog(Log::getClassLog("XmlDocument"));

//--------------------------------------------------------------------------------
QDomDocument XmlDocument::parse(const QString & xmlText)
{
    QDomDocument doc;
    QString errMsg("Parsing XML string: ");
    int errLine, errColumn;
    if (!doc.setContent(xmlText, false, &errMsg, &errLine, &errColumn)) {
        errMsg += QString(" errLine = ") + QString::number(errLine);
        errMsg += QString(" errColumn = ") + QString::number(errColumn);
        THROW_THEKLA_EXCEPTION(errMsg);
    }
    return doc;
}

//--------------------------------------------------------------------------------
QDomDocument XmlDocument::parse(QFile & file)
{
    QDomDocument doc;
    QString errMsg("Parsing file (" + file.fileName() + "): ");
    int errLine, errColumn;
    if (!doc.setContent(&file, false, &errMsg, &errLine, &errColumn)) {
        errMsg += QString(" errLine = ") + QString::number(errLine);
        errMsg += QString(" errColumn = ") + QString::number(errColumn);
        THROW_THEKLA_EXCEPTION(errMsg);
    }
    return doc;
}


//--------------------------------------------------------------------------------
XmlDocument::XmlDocument()
    : XmlSubDocument(),
      XmlFactory(),
      adapterFactory_(new QtAdapterFactory),
      isInitialized_(false)
{
}

//--------------------------------------------------------------------------------
XmlDocument::~XmlDocument()
{
    if (rootElement_ != NULL) delete rootElement_;
    delete adapterFactory_;
}


//--------------------------------------------------------------------------------
XmlSubDocument XmlDocument::createSubDocument(XmlElement * rootElement)
{
    if (rootElement == NULL || rootElement->getOwnerDocument() != this) {
        THROW_THEKLA_EXCEPTION("This Document is NOT the owner of given element.");
    }
    return XmlSubDocument(rootElement);
}

//--------------------------------------------------------------------------------
XmlElement* XmlDocument::createSubDocument(const QDomElement & domElem)
{
    XmlElement * parent = getElement(domElem.tagName());
    if (parent == NULL)
        return NULL;
    parent->init(domElem);

    QDomNodeList domChildren = domElem.childNodes();
    for (int i = 0; i < domChildren.count(); i++) {
        QDomNode domChild = domChildren.item(i);
        if (!domChild.isElement())
            continue;
        XmlElement * child = createSubDocument(domChild.toElement());
        if (child == NULL)
            continue;
        child->setParent(parent);
    }
    return parent;
}

//--------------------------------------------------------------------------------
XmlSubDocument XmlDocument::createSubDocument(const QDomDocument & domDoc)
{
    QDomElement domRootElem = domDoc.documentElement();
    XmlElement * rootElem = createSubDocument(domRootElem);
    return XmlSubDocument(rootElem);
}


//--------------------------------------------------------------------------------
XmlSubDocument XmlDocument::createSubDocument(const Observable & observable)
{
    switch (observable.type()) {

    case Observable::QOBJECT:
    {
        QObject * qObj;
        observable.getObservable(qObj);
        return createSubDocument(qObj);
    }
    default:
        XmlElement * rootElem = getElement(observable.type());
        if (rootElem == NULL)
            return NULL;

        rootElem->init(new Observable(observable));
        return XmlSubDocument(rootElem);

    }
    return XmlSubDocument();
}

//--------------------------------------------------------------------------------
XmlElement* XmlDocument::mergeSubDocument(const Observable & observable, bool syncObservableFirst)
{
    switch (observable.type()) {

    case Observable::QOBJECT:
    {
        QObject * qObj;
        observable.getObservable(qObj);
        return mergeSubDocument(qObj, syncObservableFirst);
    }
    default:
        XmlElement * elem = getElement(observable.type());
        if (elem == NULL)
            return NULL;
        try {
            elem->merge(new Observable(observable), syncObservableFirst);
        } catch (Exception & ex) {
            return NULL;
        }
        return elem;
    }
    return NULL;
}


//--------------------------------------------------------------------------------
XmlElement* XmlDocument::mergeSubDocument(QObject * qObj, bool syncObservableFirst)
{
    Log log(Log::getMethodLog(cLog, "mergeSubDocument(QObject)"));
    THEKLA_DEBUG(log, "** START. qObj = " << qObj << ", syncObservableFirst = " << syncObservableFirst);

    const QMetaObject * qMetaObj = qObj->metaObject();

    Observable qObjObs(qObj);
    QtObject * qtObject = dynamic_cast<QtObject*>(findElementExtended(qObjObs));
    if (qtObject == NULL) {
        THEKLA_ERROR(log, "Cannot find Element for Observable QObject"
                    << " (className = " << qObj->metaObject()->className()
                    << ", objectName = " << qObj->objectName().toStdString() << ").");
        return NULL;
    }

    try {
        qtObject->merge(new Observable(qObjObs), syncObservableFirst);
    } catch (std::exception & ex) {
        THEKLA_WARN(log, "Merging QtObject found Exception: " << ex.what());
    }

    //// Signals

    QList<QtSignal*> qtSignalList = qtObject->getSignals();

    for (int i = 0; i < qMetaObj->methodCount(); i++) {

        QMetaMethod qSignal = qMetaObj->method(i);
        if (qSignal.methodType() != QMetaMethod::Signal)
            continue;

        for (int j = 0; j < qtSignalList.size(); j++) {

            QtSignal * qtSignal = qtSignalList[j];

            if (QString(qSignal.signature()) == qtSignal->getSignature()) {

                try {
                    qtSignal->merge(new Observable(qObj, qSignal), syncObservableFirst);
                } catch (Exception & ex) {
                    THEKLA_ERROR(log, "Merging QtSignal found Exception: " << ex.what());
                }

                break;
            }

            if (j == qtSignalList.size()) {
                THEKLA_WARN(log, "Cannot find Element for Observable QMetaMethod = " << qSignal.signature());
            }
        }
    }

    //// Properties

    QList<QtProperty*> qtPropertyList = qtObject->getProperties();

    for (int i = 0; i < qMetaObj->propertyCount(); i++) {

        QMetaProperty qProperty = qMetaObj->property(i);

        for (int j = 0; j < qtPropertyList.size(); j++) {

            QtProperty * qtProperty = qtPropertyList[j];

            if (QString(qProperty.name()) == qtProperty->getName() &&
                QString(qProperty.typeName()) == qtProperty->getType())
            {
                try {
                    qtProperty->merge(new Observable(qObj, qProperty), syncObservableFirst);
                } catch (Exception & ex) {
                    THEKLA_ERROR(log, "Merging QtProperty found Exception: " << ex.what());
                }

                break;
            }

            if (j == qtPropertyList.size()) {
                THEKLA_WARN(log, "Cannot find Element for Observable QMetaProperty = " << qProperty.name());
            }
        }
    }

    THEKLA_DEBUG(log, "** END. ret = " << qtObject);
    return qtObject;
}


//--------------------------------------------------------------------------------
XmlSubDocument XmlDocument::createSubDocument(QObject * qObj)
{
    Log log(Log::getMethodLog(cLog, "createSubDocument(QObject)"));

    Observable qObjObs(qObj);
    XmlElement * qtObj = getElement(qObjObs.type());
    try {
        qtObj->init(new Observable(qObjObs));
    } catch (std::exception & ex) {
        THEKLA_WARN(log, "Exception caught: " << ex.what());
    }

    const QMetaObject * qMetaObj = qObj->metaObject();
    for (int i = 0; i < qMetaObj->methodCount(); i++) {

        QMetaMethod qMethod = qMetaObj->method(i);
        if (qMethod.methodType() != QMetaMethod::Signal)
            continue;

        Observable qSignalObs(qObj, qMethod);
        XmlElement * qtSignal = getElement(qSignalObs.type());
        try {
            qtSignal->setParent(qtObj);
            qtSignal->init(new Observable(qSignalObs));
        } catch (std::exception & ex) {
            THEKLA_WARN(log, "Exception caught: " << ex.what());
            delete qtSignal;
            continue;
        }

    }
    for (int i = 0; i < qMetaObj->propertyCount(); i++) {

        Observable qPropertyObs(qObj, qMetaObj->property(i));
        XmlElement * qtProperty = getElement(qPropertyObs.type());
        try {
            qtProperty->setParent(qtObj);
            qtProperty->init(new Observable(qPropertyObs));
        } catch (std::exception & ex) {
            THEKLA_WARN(log, "Exception caught: " << ex.what());
            delete qtProperty;
            continue;
        }
    }

    return XmlSubDocument(qtObj);
}



//--------------------------------------------------------------------------------
XmlElement* XmlDocument::removeElement(XmlElement * xmlElem)
{
   Log log(Log::getMethodLog(cLog, "removeElement()"));

    if (xmlElem == NULL) {
        THROW_THEKLA_EXCEPTION("Given Element is NULL.");
    }

    xmlElem->setParent(NULL);

    if (xmlElem == rootElement_)
        setRootElement(NULL);

    xmlElem->changeOwnerDocument(NULL, true);

    return xmlElem;
}


//--------------------------------------------------------------------------------
XmlElement* XmlDocument::addElement(XmlElement * xmlElem)
{
    Log log(Log::getMethodLog(cLog, "addElement()"));

    //// check parameters

    if (xmlElem == NULL) {
        THROW_THEKLA_EXCEPTION("Given Element equals NULL.");
    }

    //// find duplicate within DOM

    QList<XmlElement*> duplicateElemList = findElements(xmlElem->getXmlData());

    for (int i = 0; i < duplicateElemList.size(); i++) {

        //// Found exact (!) duplicate element

        if (duplicateElemList[i] == xmlElem) {
            return xmlElem;
        }

        //// Found duplicate element

        else {

//             //// merge elements

//             // merge xml data
//             xmlElem->merge(duplicateElem->getXmlData());

//             // re-hook duplicate children given element children
//             for (int i = 0; i < duplicateElem->getChildCount(); i++) {
//                 duplicateElem->getChild(i)->setParent(xmlElem);
//                 delete duplicateElem;
//             }

            THROW_THEKLA_EXCEPTION("Found duplicate Element within DOM."
                                  << "Given Element does not have equal pointer value ("
                                  << duplicateElemList[i] << " != " << xmlElem << ").");
        }
    }

    //// find parent element

    XmlElementInfo * info = getInfo(xmlElem->getXmlData().getTagName());
    if (info == NULL) {
        QString tagName = xmlElem->getXmlData().getTagName();
        THROW_THEKLA_EXCEPTION("Could not retrieve ElementInfo for tagName (" << tagName << ").");
    }

    //// set as root element

    if (info->getParentInfo() == NULL) {

        // ERROR: cannot replace current root element
        if (rootElement_ != NULL) {
            THROW_THEKLA_EXCEPTION("Cannot insert Element as root element: Current root element exists and cannot be removed.");
        }

        // set as root element
        setRootElement(xmlElem);

    }

    //// add as child of parent element

    else {

        // find (first) parent element
        XmlElement * parent = findFirstElement(info->getParentInfo()->getTagName());

        // ERROR: cannot find parent element
        if (parent == NULL) {
            THROW_THEKLA_EXCEPTION("Cannot find parent element (NULL) of given Element.");
        }

        // set as child of parent element
        xmlElem->setParent(parent);
    }

    //// set actual document as owner document

    xmlElem->changeOwnerDocument(this, true);

    return xmlElem;
}

//--------------------------------------------------------------------------------
XmlElement * XmlDocument::findElementExtended(const Observable & observable) const
{
    Log log(Log::getMethodLog(cLog, "findElementExtended(Observable)"));
    THEKLA_DEBUG(log, "** START. observable.type() = " << observable.type());

    XmlElement * ret = NULL;

    // (1) search for the observable directly
    ret = XmlSubDocument::findElement(observable);
    if (ret != NULL) {
        THEKLA_DEBUG(log, "** END. Found exact match.");
        return ret;
    }

    // (2) search with xml representation of the observable

    // create xml representation
    XmlElement * cmp = getElement(observable.type());
    if (cmp == NULL) {
        THEKLA_DEBUG(log, "** END: Factory does not know Observable type: " << observable.type());
        return NULL;
    }

    THEKLA_DEBUG(log, "Comparator element != NULL");

    try {
        cmp->init(new Observable(observable));
    } catch (std::exception & ex) {
        THEKLA_WARN(log, "Init Element found exception: " << ex.what());
        delete cmp; return NULL;
    }

    ret = findFirstElement(cmp->getXmlData());

    // delete the comparator element
    delete cmp;

    THEKLA_DEBUG(log, "** END. ret = " << ret);
    return ret;
}

//--------------------------------------------------------------------------------
void XmlDocument::mergeDocument(QObject * qObjMainContainer, bool syncObservableFirst)
{
    Log log(Log::getMethodLog(cLog, "mergeDocument()"));
    THEKLA_DEBUG(log, "** START.");

    if (qObjMainContainer == NULL) {
        THEKLA_WARN(log, "** END. Given main container QObject == NULL.");
        return;
    }

    //// merge QtObject SubDocuments

    QList<XmlElement*> qtObjectList = findElements("QtObject");

    THEKLA_DEBUG(log, "-- qtObjectList.size() = " << qtObjectList.size());

    for (int i = 0; i < qtObjectList.size(); i++) {

        // find Observable within FormWindow
        QtObject * qtObject = dynamic_cast<QtObject*>(qtObjectList[i]);

        QList<QObject*> qObjectList = qObjMainContainer->findChildren<QObject*>(qtObject->getObjectName());

        for (int i = 0; i < qObjectList.size(); i++) {
            QObject * qObject = qObjectList[i];
            if (QString(qObject->metaObject()->className()).remove("Designer") != qtObject->getClassName()) {
                qObjectList.removeAt(i);
                i--;
            }
        }

        // Expecting exactly ONE widget in return

        if (qObjectList.size() != 1) {

            // more than one QWidget found
            if (qObjectList.size() > 1) {
                THEKLA_WARN(log, "Ambiguous QObject List. qObjectList.size() = " << qObjectList.size() << ".");
                // TODO: Dialog box, some reaction ?!
            }

            // no QWidget found
            THEKLA_WARN(log, "EMPTY QObject List. qObjectList.size() = " << qObjectList.size() << ".");

            // delete the element from the document
            XmlElement * tmp = removeElement(qtObject);
            delete tmp;
            continue;
        }

        // NOTE: Attention (!) this would be intuitive -> BUT
//        qtObject->merge(new Observable(qObject));
        // .. this is the designated use .. hmmm
        mergeSubDocument(Observable(qObjectList[0]), syncObservableFirst);
    }

    THEKLA_DEBUG(log, "** END.");
}

//--------------------------------------------------------------------------------
void XmlDocument::updateDocument()
{
    Log log(Log::getMethodLog(cLog, "updateDocument()"));
    THEKLA_DEBUG(log, "** START.");

    //// update elements (sync with Observables)

    updateSubDocument(getRootElement());

//     QList<XmlElement*> qtObjectList = findElements("QtObject");

//     for (int i = 0; i < qtObjectList.size(); i++) {

//         // update QtObject
//         QtObject * qtObject = dynamic_cast<QtObject*>(qtObjectList[i]);
//         qtObject->update();

//         // update QtSignal
//         QList<QtSignal*> qtSignalList = qtObject->getSignals();
//         for (int j = 0; j < qtSignalList.size(); j++) {
//             qtSignalList[j]->update();
//         }

//         // update QtProperties
//         QList<QtProperty*> qtPropertyList = qtObject->getProperties();
//         for (int j = 0; j < qtPropertyList.size(); j++) {
//             qtPropertyList[j]->update();
//         }
//     }

//     QList<XmlElement*> oivFieldList = findElements("OivField");

//     for (int i = 0; i < oivFieldList.size(); i++) {
//         oivFieldList[i]->update();
//     }

    THEKLA_DEBUG(log, "** END.");
}

//--------------------------------------------------------------------------------
void XmlDocument::updateSubDocument(XmlElement * xmlElem)
{
    // update parent
    xmlElem->update();

    // process children
    for (int i = 0; i < xmlElem->getChildCount(); i++) {
        updateSubDocument(xmlElem->getChild(i));
    }
}


//--------------------------------------------------------------------------------
void XmlDocument::syncObservableSubDocument(XmlElement * xmlElem)
{
    if (xmlElem == NULL)
        return;

    // sync observable of current element
    xmlElem->syncObservable();

    // process children elements
    for (int i = 0; i < xmlElem->getChildCount(); i++) {
        syncObservableSubDocument(xmlElem->getChild(i));
    }
}

//--------------------------------------------------------------------------------
XmlElement* XmlDocument::cloneSubDocumentAux(XmlElement * xmlElem)
{
    XmlElement * xmlElemClone = xmlElem->clone();

    for (int i = 0; i < xmlElem->getChildCount(); i++) {
        XmlElement * xmlElemChildClone = cloneSubDocumentAux(xmlElem->getChild(i));
        xmlElemChildClone->setParent(xmlElemClone);
    }

    return xmlElemClone;
}

//--------------------------------------------------------------------------------
XmlSubDocument XmlDocument::cloneSubDocument(XmlElement * rootElement)
{
    if (rootElement == NULL)
        return XmlSubDocument(NULL);

    return XmlSubDocument(cloneSubDocumentAux(rootElement));
}

