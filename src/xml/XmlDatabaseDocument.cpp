
#include "XmlDatabaseDocument.h"

#include <common/DatabaseConn.h>
#include <common/Exception.h>
#include <common/Log.h>

#include <xml/UIDataRoot.h>
#include <xml/DBRootElement.h>
#include <xml/UISettings.h>
#include <xml/XmlConfigFileDocument.h>
#include <xml/XmlElementData.h>
#include <xml/XmlElementInfo.h>
#include <xml/QtObject.h>
#include <xml/QtSignal.h>
#include <xml/QtProperty.h>
#include <xml/OivField.h>


//================================================================================

//--------------------------------------------------------------------------------
Log XmlDatabaseDocument::cLog(Log::getClassLog("XmlDatabaseDocument"));
const QString XmlDatabaseDocument::DEFAULT_UI_DATA_XPATH_PREFIX("/THEKLA");

//--------------------------------------------------------------------------------
XmlDatabaseDocument::XmlDatabaseDocument(DatabaseConn & dbConn)
    : XmlDocument(),
      dbConn_(dbConn)
{
}

//--------------------------------------------------------------------------------
XmlDatabaseDocument::~XmlDatabaseDocument()
{
}

//--------------------------------------------------------------------------------
void XmlDatabaseDocument::init(const QString & appId, const QString & uiDataRootXPathPrefix)
{
    if (isInitialized_)
        return;

    //// check UI data XPath prefix

    if (!uiDataRootXPathPrefix.startsWith('/') ||
        uiDataRootXPathPrefix.indexOf('/', 1) == 1 ||
        uiDataRootXPathPrefix.endsWith('/'))
    {
        THROW_THEKLA_EXCEPTION("Invalid UI data root XPath prefix "
                               << "(" << uiDataRootXPathPrefix << "). "
                               << "NOTES: XPath must NOT start with wildcard '//' (instead MUST start with '/')"
                               << " and must NOT end with '/'. Example: " << DEFAULT_UI_DATA_XPATH_PREFIX);
    }

    if (!dbConn_.exists(uiDataRootXPathPrefix)) {
        THROW_THEKLA_EXCEPTION("Cannot find UI data root element using XPath prefix "
                               << "(" << uiDataRootXPathPrefix << ").");
    }

    //// register additional Elements

    // remove starting slashes: "/THEKLA" -> "THEKLA"
    QString uiDataRootXPathPrefixTagName(uiDataRootXPathPrefix);
    while (uiDataRootXPathPrefixTagName.startsWith('/')) {
        uiDataRootXPathPrefixTagName = uiDataRootXPathPrefixTagName.remove(0, 1);
    }

    // DB root element
    registerDefaultElements();
    XmlElementInfo * dbRootInfo = registerElement(uiDataRootXPathPrefixTagName, new DBRootElement, NULL);
//    XmlElementInfo * uiDataRootInfo =
        registerElement("TheklaUIData", new UIDataRoot, dbRootInfo);

    //// add Root (DB + UI) Elements

    // add root element to DOM
    XmlElement * dbRoot = getElement(uiDataRootXPathPrefixTagName);
    setRootElement(dbRoot);

    // add THEKLA root element (using the given application id) to DOM and into DB
    UIDataRoot * uiDataRoot = dynamic_cast<UIDataRoot*>(getElement("TheklaUIData"));
    uiDataRoot->setAppId(appId);
    addElement(uiDataRoot);

    isInitialized_ = true;
}


//--------------------------------------------------------------------------------
void XmlDatabaseDocument::init(const XmlConfigFileDocument & fileDoc, bool mergeSubDocWithDb)
{
    Log log(Log::getMethodLog(cLog, "init()"));
    THEKLA_DEBUG(log, "** START.");

    //// create root element

    UIDataRoot * uiDataRoot = dynamic_cast<UIDataRoot*>(fileDoc.getUIDataRoot());
    if (uiDataRoot == NULL)
        THROW_THEKLA_EXCEPTION("XmlConfigFileDocument contains UIDataRoot (NULL).");
    UISettings * uiSettings = dynamic_cast<UISettings*>(fileDoc.getUISettings());
    if (uiSettings == NULL)
        THROW_THEKLA_EXCEPTION("XmlConfigFileDocument contains UISettings (NULL).");

    init(uiDataRoot->getAppId(), uiSettings->getUIDataXPathPrefix());

    //// create QtObject elements

    QList<XmlElement*> qtObjList = fileDoc.findElements("QtObject");

    for (int i = 0; i < qtObjList.size(); i++) {

        XmlSubDocument subDoc = createSubDocument(qtObjList[i]->getDom());

        QtObject * qtObj = dynamic_cast<QtObject*>(subDoc.getRootElement());

        //// filter Signals + Properties

        QList<QtSignal*> qtSignalList = qtObj->getSignals();
        for (int j = 0; j < qtSignalList.size(); j++) {

            QtSignal * qtSignal = qtSignalList[j];

            if (!qtSignal->getXmlData().getConfigFlag(XmlElementData::BIND) &&
                !qtSignal->getXmlData().getConfigFlag(XmlElementData::USE_AS_TRIGGER))
            {
                delete qtSignal;
            }
        }


        QList<QtProperty*> qtPropertyList = qtObj->getProperties();
        for (int j = 0; j < qtPropertyList.size(); j++) {

            QtProperty * qtProperty = qtPropertyList[j];

            if (!qtProperty->getXmlData().getConfigFlag(XmlElementData::UPDATE))
            {
                delete qtProperty;
            }
        }
        addElement(qtObj, mergeSubDocWithDb);

    }
}

//--------------------------------------------------------------------------------
void XmlDatabaseDocument::applyConfiguration()
{
    Log log(Log::getMethodLog(cLog, "applyConfiguration()"));
    THEKLA_DEBUG(log, "** START");


    //// bind the QtObject signals + properties

    QList<XmlElement*> qtObjElemList = findElements("QtObject");
    THEKLA_DEBUG(log, "qtObjElemList.size() = " << qtObjElemList.size());

    for (int i = 0; i < qtObjElemList.size(); i++) {

        QtObject * qtObject = dynamic_cast<QtObject*>(qtObjElemList[i]);
        qtObject->bindConfiguredSignals();
        qtObject->bindConfiguredProperties();
    }


    //// bind the OivFields

    QList<XmlElement*> oivFieldElemList = findElements("OivField");
    THEKLA_DEBUG(log, "oivFieldElemList.size() = " << oivFieldElemList.size());

    for (int i = 0; i < oivFieldElemList.size(); i++) {

        OivField * oivField = dynamic_cast<OivField*>(oivFieldElemList[i]);
        oivField->bind();
    }

    THEKLA_DEBUG(log, "** END");
}

//--------------------------------------------------------------------------------
XmlElement* XmlDatabaseDocument::addElement(XmlElement * xmlElem)
{
    return addElement(xmlElem, false);
}

//--------------------------------------------------------------------------------
XmlElement* XmlDatabaseDocument::addElement(XmlElement * xmlElem, bool mergeSubDocWithDb)
{
    XmlElement * rootElemAdded = XmlDocument::addElement(xmlElem);

    if (rootElemAdded != NULL) {

        if (mergeSubDocWithDb) {
            insertIntoOrMergeDbRecursive(*rootElemAdded);
        } else if (xmlElem->hasObservable()) {
            insertIntoOrUpdateDbRecursive(*rootElemAdded);
        } else {
            insertIntoDbRecursive(*rootElemAdded);
        }
    }

    return rootElemAdded;
}


#if 0
//--------------------------------------------------------------------------------
XmlElement* XmlDatabaseInputDocument::removeElement(XmlElement * xmlElem)
{
    XmlElement * rootElemRemoved = XmlDocument::removeElement(xmlElem);

    if (rootElemRemoved != NULL) {
        dbConn_.deleteFrom(*rootElemRemoved);
    }

    return rootElemRemoved;
}
#endif


//--------------------------------------------------------------------------------
UIDataRoot* XmlDatabaseDocument::getUIDataRoot() const
{
    return dynamic_cast<UIDataRoot*>(findFirstElement(QString("TheklaUIData")));
}

//--------------------------------------------------------------------------------
void XmlDatabaseDocument::mergeWithDbRecursive(XmlElement & xmlElem)
{
    // merge element with db (db -> element)
    xmlElem.merge(dbConn_);

    // process children elements
    for (int i = 0; i < xmlElem.getChildCount(); i++) {
        mergeWithDbRecursive(*(xmlElem.getChild(i)));
    }
}

//--------------------------------------------------------------------------------
void XmlDatabaseDocument::insertIntoDbRecursive(const XmlElement & xmlElem)
{
    // insert element into DB (and stop)
    if (!dbConn_.exists(xmlElem)) {
        dbConn_.insert(xmlElem);
        return;
    }

    // process children elements
    for (int i = 0; i < xmlElem.getChildCount(); i++) {
        insertIntoDbRecursive(*(xmlElem.getChild(i)));
    }
}


//--------------------------------------------------------------------------------
void XmlDatabaseDocument::insertIntoOrUpdateDbRecursive(XmlElement & xmlElem)
{
    // insert element into DB (and stop)
    if (!dbConn_.exists(xmlElem)) {
        dbConn_.insert(xmlElem);
        return;
    }
    // update element in DB (but continue!)
    else {
        // see NOTE about MW-update above: I assume for now, that update also INSERTs
        // elements (that's why we don't need to continue with child elements stop here).

        // NOTE2: DELETE elements is forbidden, so only leafs are updated.
        if (xmlElem.isLeaf()) {
            dbConn_.update(xmlElem);
            return;
        }
    }

    // process children elements
    for (int i = 0; i < xmlElem.getChildCount(); i++) {
        insertIntoOrUpdateDbRecursive(*(xmlElem.getChild(i)));
    }
}

//--------------------------------------------------------------------------------X
void XmlDatabaseDocument::insertIntoOrMergeDbRecursive(XmlElement & xmlElem)
{
    Log log(Log::getMethodLog(cLog, "insertIntoOrMergeDbRecursive()"));
    THEKLA_DEBUG(log, "** START. XmlElement.getText() = " << xmlElem.getXmlData().getText().data());

    // insert element into DB (and stop)
    if (!dbConn_.exists(xmlElem)) {
        dbConn_.insert(xmlElem);
        THEKLA_DEBUG(log, "** END. XmlElement does NOT exist. INSERTED.");
        return;
    }
    // merge element with DB (but continue!)
    else {
        THEKLA_DEBUG(log, "XmlElement DOES exist. MERGE.");
        xmlElem.merge(dbConn_);
    }

    // process children elements
    for (int i = 0; i < xmlElem.getChildCount(); i++) {
        insertIntoOrMergeDbRecursive(*(xmlElem.getChild(i)));
    }
    THEKLA_DEBUG(log, "** END. XmlElement.getText() = " << xmlElem.getXmlData().getText().data());
}
