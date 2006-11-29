
#include "XmlTrashDocument.h"
#include "XmlElement.h"
#include "QtObject.h"
#include <common/QtObjectUtils.h>
#include <common/Log.h>

//--------------------------------------------------------------------------------
Log XmlTrashDocument::cLog(Log::getClassLog("XmlTrashDocument"));

//--------------------------------------------------------------------------------
XmlTrashDocument::XmlTrashDocument()
    : XmlDocument()
{
    //// register / unregister elements

    registerDefaultElements();

    unregisterElement(getInfo("TheklaUIData"));
    unregisterElement(getInfo("OivField"));

    XmlElementInfo * infoSep = registerElement("Separator", new XmlElement(false), NULL);
    registerElement("QtObject", new QtObject, infoSep);

    //// init DOM

    XmlElement * sepElem = getElement("Separator");
    setRootElement(sepElem);

    isInitialized_ = true;
}

//--------------------------------------------------------------------------------
XmlTrashDocument::~XmlTrashDocument()
{
}

//--------------------------------------------------------------------------------
void XmlTrashDocument::push(QtObject * qtObj)
{
    Log log(Log::getMethodLog(cLog, "addElement(QtObject)"));
    THEKLA_DEBUG(log, "** START.");

    // avoid collisions within the trash document by adding a unique id
    qtObj->getXmlData().setAttribute("trash-id", QtObjectUtils::getUniqueIdentifier());

    // add to trash document
    XmlDocument::addElement(qtObj);
}

//--------------------------------------------------------------------------------
QtObject* XmlTrashDocument::get(QObject * qObject)
{
    Log log(Log::getMethodLog(cLog, "removeElement(QObject)"));
    THEKLA_DEBUG(log, "** START. QObject (className = " << qObject->metaObject()->className()
                << ", objectName = " << qObject->objectName().toStdString() << ").");

    QString qObjClassName(qObject->metaObject()->className());
    qObjClassName.remove("Designer");

    //// Find a corresponding Element within Trash

    QtObject * qtObject = NULL;

    // get all QtObject elements
    QList<XmlElement*> xmlElemList = findElements("QtObject");

    THEKLA_WARN(log, "xmlElemList.size() = " << xmlElemList.size());

    // filter QtObject elements which do a have different class name
    for (int i = 0; i < xmlElemList.size(); i++) {
        qtObject = dynamic_cast<QtObject*>(xmlElemList[i]);
        if (qtObject == NULL) {
            THEKLA_WARN(log, "Found non-QtObject with \"QtObject\" tagname.");
            continue;
        }
        // filter the QtObject elements that do not have the proper class name
        if (qtObject->getClassName() != qObjClassName) {
            xmlElemList.removeAt(i);
            i--;
        }
        // NOTE: MUST NOT filter the QtObject elements that do not have the proper object
        // name, because it is a FACT, that the given Observable object have different
        // names compared to all elements that have been added to the trash.

        // Examples: [ "QPushButton", "pushButton" ] removed from from -> added to
        // trash. Then: This button is re-added to the form TWICE, once with the object
        // name "pushButton" and once with the (unique - QDesigner takes care of that)
        // name "pushButton_2". The given Observable has EXACTLY that name (thanx to
        // QDesinger), but the (correct) Trash element still has "pushButton" as its
        // name. Renaming -> see below..
    }
    THEKLA_WARN(log, "xmlElemList.size() = " << xmlElemList.size() << ", after cleanup.");

    // QtObject element list empty, nothing found in Trash
    if (xmlElemList.size() == 0) {
        THEKLA_DEBUG(log, "** END: Given QObject NOT FOUND in Trash.");
        return NULL;
    }

    // Magic: take the LAST element of the QtObject Element list. Why? Because the
    // Document works as queue: QtObjects are inserted and put on a element child list
    // (using push_back()). So if we want to most recent acquisition of trash, we take the
    // last child of the list (clearly a dirty hack..). In short: the QtElements as
    // children of "Separator" form a LIFO queue.

    // NOTE: never remove elements from the TrashDocument. This is done automatically when
    // the Observable of the QtObject (that is a QObject) gets deleted. Because the
    // QtObject is registered as child of its Observable, and gets deleted as well (thanx
    // to Qt). IMPORTANT 1: Never exchange the Observable of a QtObject.

    qtObject = dynamic_cast<QtObject*>(xmlElemList.last());

    //// check QtObject that we have found in Trash

    if (qtObject == NULL) {
        THEKLA_DEBUG(log, "** END: dynamic_cast() == NULL.");
        return NULL;
    }

    // generate a clone of the QtObject sub-document (with all signals and properties)
    XmlSubDocument subDocClone = cloneSubDocument(qtObject);
    QtObject * qtObjectClone = dynamic_cast<QtObject*>(subDocClone.getRootElement());

    // Create a (auxiliary) Document because we'll need it ..
    XmlTrashDocument docClone;
    docClone.setRootElement(qtObjectClone);

    // set the correct object name (see NOTE above)
    qtObjectClone->setObjectName(qObject->objectName());

    // .. when we want to merge the given QObject Observable with the QtObject (plus its
    // QtProperties, QtSignal children)
    docClone.mergeSubDocument(Observable(qObject));
    // IMPORTANT: reset the document root element to NULL, otherwise the elements are
    // deleted when the document is deleted
    docClone.setRootElement(NULL);

    // remove trash id
    qtObjectClone->getXmlData().removeAttribute("trash-id");

    THEKLA_DEBUG(log, "** END. Ok.");
    return qtObjectClone;
}

