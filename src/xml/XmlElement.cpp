
#include "XmlElement.h"

#include "XmlElementData.h"
#include "XmlDocument.h"
#include <common/Exception.h>
#include <common/Log.h>
#include <common/Observer.h>
#include <common/DatabaseConn.h>
#include "XmlDatabaseDocument.h"

//--------------------------------------------------------------------------------
Log XmlElement::cLog(Log::getClassLog("XmlElement"));

//--------------------------------------------------------------------------------
XmlElement::XmlElement(bool containsText)
    : state_(0),
      xmlData_(new XmlElementData(containsText)),
      ownerDoc_(NULL),
      parent_(NULL),
      children_(),
      observable_(NULL),
      observerList_(),
      watchdogId_(DatabaseConn::MIN_WATCHDOG_ID - 1)
{
}

//--------------------------------------------------------------------------------
XmlElement::XmlElement(const XmlElement & copy)
    : xmlData_(new XmlElementData(*(copy.xmlData_))),
      ownerDoc_(copy.ownerDoc_),
      parent_(NULL),
      children_(),
      observable_(NULL),
      observerList_(),
      watchdogId_(DatabaseConn::MIN_WATCHDOG_ID - 1)
{
}

//--------------------------------------------------------------------------------
XmlElement::~XmlElement()
{
    Log log(Log::getMethodLog(cLog, "~XmlElement()", log4cplus::OFF_LOG_LEVEL));
    THEKLA_DEBUG(log, "** START.");

    // Watchdog
    if (hasWatchdog())
        unregisterWatchdog();

    // delete child elements
    while (getChildCount() != 0) {
        // fetch from child list
        XmlElement * child = getChild(0);
        // remove from child list, resets itself as parent
        child->setParent(NULL);
        // delete child
        delete child;
    }
    children_.clear();

    // removes itself from child list of its parent
    setParent(NULL);

    // Element data
    delete xmlData_;

    // Observable
    if (observable_ != NULL) delete observable_;

    // Observers
    for (int i = 0; i < observerList_.size(); i++) {
        const Observer * obs = observerList_.value(i);
        delete obs;
    }
    observerList_.clear();

    THEKLA_DEBUG(log, "** END.");
}

//--------------------------------------------------------------------------------
QString XmlElement::getXPath() const
{
    QString xPath = "";
    if (parent_ != NULL) xPath += parent_->getXPath();
    xPath += xmlData_->getXPath();
    return xPath;
}

//--------------------------------------------------------------------------------
bool XmlElement::changeOwnerDocument(XmlDocument * ownerDoc, bool applyRecursive)
{
    bool ret = true;

    // set owner document
    ownerDoc_ = ownerDoc;

    // apply on child elements
    if (applyRecursive) {
        for (int i = 0; i < getChildCount(); i++) {
            ret &= getChild(i)->changeOwnerDocument(ownerDoc, applyRecursive);
        }
    }

    return ret;
}

//--------------------------------------------------------------------------------
QDomDocument XmlElement::getDom() const
{
    QDomDocument doc = xmlData_->getDom();
    QDomElement rootElem = doc.documentElement();
    for (int i=0; i < getChildCount(); i++) {
        QDomDocument childDoc = getChild(i)->getDom();
        rootElem.appendChild(childDoc.documentElement());
    }
    return doc;
}

//--------------------------------------------------------------------------------
XmlElement* XmlElement::setParent(XmlElement * parent)
{
    Log log(Log::getMethodLog(cLog, "setParent()"));

    // not allowed to set itself as parent
    if (this == parent)
        return NULL;
    // not allowed to set the same parent twice
    if (parent_ == parent)
        return parent_;


    if (parent == NULL) {
        // remove itself from children list of parent
        int removedCount = parent_->children_.removeAll(this);
        if (removedCount != 1)
            THEKLA_ERROR(log, "removedCount = " << removedCount << " (!= 1)");
    } else {
        // add itself in children list of parent
        parent->children_.push_back(this);
    }

    // set given parent and return previous parent
    XmlElement * ret = parent_;
    parent_ = parent;
    return ret;
}

//--------------------------------------------------------------------------------
void XmlElement::init(const QDomElement & domElem)
{
    // create new xml data
    XmlElementData data(domElem);

    // overwrite old one
    xmlData_->clear();
    xmlData_->setTagName(data.getTagName());
    xmlData_->merge(data);
}


//--------------------------------------------------------------------------------
void XmlElement::init(const Observable * observable)
{
    // Check given obserable
    if (observable_ != NULL) {
        THROW_THEKLA_EXCEPTION("Does already have an Obserable. Repeatedly init not possible.");
    }
    if (!canInit(observable->type())) {
        THROW_THEKLA_EXCEPTION("Given observable type (" << observable->type() << ") not supported.");
    }
    if (observable->isNull()) {
        THROW_THEKLA_EXCEPTION("Observable contains NULL pointer values.");
    }
    if (!observable->isSerializable()) {
        THROW_THEKLA_EXCEPTION("Observable object cannot be serialized.");
    }

    // Store given obserable
    observable_ = observable;
}


//--------------------------------------------------------------------------------
void XmlElement::merge(const Observable * observable, bool syncObservableFirst)
{
    // base class does not know how to sychronize w/ Observable
    Q_UNUSED(syncObservableFirst);

    // delete current observable
    if (observable_ != NULL) {
        delete observable_;
        observable_ = NULL;
    }

    try {
        // init with given Observable object (calls specialized method)
        init(observable);
    } catch(Exception & ex) {

        // reset Observable
        if (observable_ != NULL) {
            observable_ = NULL;
            // NOTE: do not kill the observable (it's not ours)
        }

        // re-throw exception
        throw Exception(ex);
    }
}
//--------------------------------------------------------------------------------
void XmlElement::bind()
{
    //// Prepare

    XmlElement::bindBefore();

    //// Specific

    THROW_THEKLA_EXCEPTION("This Element knows no strategy to be bound to its Observable.");

    //// Finalize

    bindAfter();
}


//--------------------------------------------------------------------------------
void XmlElement::bindBefore()
{
    if (observable_ == NULL) {
        THROW_THEKLA_EXCEPTION("Given Observable is NULL.");
    }
}

//--------------------------------------------------------------------------------
void XmlElement::bindAfter()
{
    Log log(Log::getMethodLog(cLog, "bindAfter()"));

    enableState(BOUND_TO_OBSERVABLE, true);

    //// First Observer of the Element is the Observable itself

    Observer * obs = new Observer(*observable_);

    if (obs->isConnectable()) {
        try {
            connect(obs);
            THEKLA_DEBUG(log, "Connect Observable as Observer: OK."
                         << " (tagName = " << xmlData_->getTagName().toStdString() << ")");
        } catch (Exception & ex) {
            THEKLA_WARN(log, "Connect Observable as Observer: NOK."
                        << " (tagName = " << xmlData_->getTagName().toStdString() << ")");
            delete obs;
        }
    }
}

//--------------------------------------------------------------------------------
void XmlElement::merge(const QDomElement & domElem)
{
    XmlElementData xmlDataMerge(domElem);

    // do nothing if tag names do not match
    if (xmlData_->getTagName() != xmlDataMerge.getTagName())
        return;

    merge(xmlDataMerge);
}


//--------------------------------------------------------------------------------
void XmlElement::merge(const XmlElementData & xmlData)
{
    xmlData_->merge(xmlData);
}


//--------------------------------------------------------------------------------
bool XmlElement::operator==(const Observable & observable) const
{
    if (observable_ == NULL)
        return false;
    return ((*observable_) == observable);
}

//--------------------------------------------------------------------------------
XmlElement* XmlElement::clone() const
{
    return new XmlElement(*this);
}

//--------------------------------------------------------------------------------
XmlElement* XmlElement::findFirstElement(const QString & tagName)
{
    if (xmlData_->getTagName() == tagName)
        return this;
    for (int i = 0; i < getChildCount(); i++) {
        XmlElement * ret = NULL;
        if ((ret = getChild(i)->findFirstElement(tagName)) != NULL)
            return ret;
    }
    return NULL;
}

//--------------------------------------------------------------------------------
XmlElement* XmlElement::findFirstElement(const XmlElementData & primaryKey)
{
    if (xmlData_->contains(primaryKey))
        return this;
    for (int i = 0; i < getChildCount(); i++) {
        XmlElement * ret = NULL;
        if ((ret = getChild(i)->findFirstElement(primaryKey)) != NULL)
            return ret;
    }
    return NULL;
}

//--------------------------------------------------------------------------------
QList<XmlElement*> XmlElement::findElements(const XmlElementData & primaryKey)
{
    QList<XmlElement*> elemList;

    if (xmlData_->contains(primaryKey)) {
        elemList.push_back(this);
    }
    for (int i = 0; i < getChildCount(); i++) {
        QList<XmlElement*> tmp = getChild(i)->findElements(primaryKey);
        for (int j = 0; j < tmp.size(); j++)
            elemList.push_back(tmp.value(j));
    }
    return elemList;
}


//--------------------------------------------------------------------------------
QList<XmlElement*> XmlElement::findElements(const QString & tagName)
{
    QList<XmlElement*> elemList;
    if (xmlData_->getTagName() == tagName)
        elemList.push_back(this);
    for (int i = 0; i < getChildCount(); i++) {
        QList<XmlElement*> ret;
        ret = getChild(i)->findElements(tagName);
        for (int j = 0; j < ret.size(); j++)
            elemList.push_back(ret.value(j));
    }
    return elemList;

}

//--------------------------------------------------------------------------------
XmlElement* XmlElement::findElement(const Observable & observable)
{
    const Observable * obs = getObservable();
    if (obs != NULL && ((*obs) == observable))
        return this;
    for (int i = 0; i < getChildCount(); i++) {
        XmlElement * ret = NULL;
        if ((ret = getChild(i)->findElement(observable)) != NULL)
            return ret;
    }
    return NULL;

}

//--------------------------------------------------------------------------------
DatabaseConn* XmlElement::getDatabaseConn() const
{
    XmlDatabaseDocument * dataDoc = dynamic_cast<XmlDatabaseDocument*>(ownerDoc_);
    if (dataDoc == NULL)
        return NULL;
    return &(dataDoc->getDatabaseConn());
}

//--------------------------------------------------------------------------------
void XmlElement::insertIntoDB() const
{
    DatabaseConn * dbConn = getDatabaseConn();
    if (dbConn == NULL)
        return;

    if (dbConn->exists(*this))
        return;

    if (!dbConn->insert(*this))
        THROW_THEKLA_EXCEPTION("InsertIntoDb(): DatabaseConn::insert() failed.");
}


//--------------------------------------------------------------------------------
void XmlElement::updateSetDB() const
{
    if (!isLeaf())
        return;

    DatabaseConn * dbConn = getDatabaseConn();
    if (dbConn == NULL)
        return;

    dbConn->update(*this);
}

//--------------------------------------------------------------------------------
void XmlElement::updateSetDB(const QList<XmlElement *> & xmlElemList) const
{
    DatabaseConn * dbConn = getDatabaseConn();
    if (dbConn == NULL)
        return;

    if (!dbConn->update(xmlElemList))
        THROW_THEKLA_EXCEPTION("Update DB (List) failed.");
}

//--------------------------------------------------------------------------------
void XmlElement::registerWatchdog()
{
    DatabaseConn * dbConn = getDatabaseConn();
    if (dbConn == NULL)
        return;

    if (hasWatchdog())
        return;

    watchdogId_ = dbConn->registerWatchdog(*this);
}

//--------------------------------------------------------------------------------
void XmlElement::unregisterWatchdog()
{
    DatabaseConn * dbConn = getDatabaseConn();
    if (dbConn == NULL)
        return;

    if (!hasWatchdog())
        return;

    dbConn->unregisterWatchdog(*this);
    watchdogId_ = DatabaseConn::MIN_WATCHDOG_ID - 1;
}

//--------------------------------------------------------------------------------
bool XmlElement::hasWatchdog() const
{
    return (watchdogId_ >= DatabaseConn::MIN_WATCHDOG_ID);
}

//--------------------------------------------------------------------------------
void XmlElement::merge(const DatabaseConn & dbConn)
{
    if (!dbConn.exists(*this))
        return;

    QDomDocument doc = dbConn.select(getXPath());
    merge(doc.documentElement());
}

//--------------------------------------------------------------------------------
void XmlElement::connect(const Observer * observer)
{
    connectBefore(observer);

    switch (observer->type())
    {
    default:
        THROW_THEKLA_EXCEPTION("Given Observer type (" << observer->type() << ") is not supported by this Element");
    }

    connectAfter(observer);
}

//--------------------------------------------------------------------------------
void XmlElement::connectBefore(const Observer * observer)
{
    // Check given Observer object
    if (observer == NULL) {
        THROW_THEKLA_EXCEPTION("Given Observer pointer is NULL.");
    }
    if (observer->isNull()) {
        THROW_THEKLA_EXCEPTION("Given Observer contains NULL pointer values.");
    }
    if (!observer->isConnectable()) {
        THROW_THEKLA_EXCEPTION("Given Observer cannot be connected.");
    }
}

//--------------------------------------------------------------------------------
void XmlElement::connectAfter(const Observer * observer)
{
    Log log(Log::getMethodLog(cLog, "connectAfter()"));

    // Store observer
    observerList_.push_back(observer);

    // Register watchdog
    if (!hasWatchdog()) {
        registerWatchdog();
    }

    // Merge with database
    DatabaseConn * dbConn = getDatabaseConn();
    if (dbConn != NULL) {
        merge(*dbConn);
    }
    THEKLA_DEBUG(log, "getText() = " << xmlData_->getText().data());
    // Synchronize Observers (But ONLY values, no triggers)
    if (xmlData_->getText().isEmpty() == false) {
        sync();
    }

}

//--------------------------------------------------------------------------------
bool XmlElement::isLeaf() const
{
    return (getChildCount() == 0);
}

//--------------------------------------------------------------------------------
void XmlElement::update()
{
    updateSetDB();
}

//--------------------------------------------------------------------------------
void XmlElement::enableState(State flag, bool enable)
{
    if (enable) { state_ |= flag; }
    else { state_ &= ~flag; }
}

//--------------------------------------------------------------------------------
void XmlElement::dyingObservable()
{
    if (isStateEnabled(BOUND_TO_OBSERVABLE))
        unbind();

    delete observable_;
    observable_ = NULL;
}

//--------------------------------------------------------------------------------
void XmlElement::dyingObserver(const Observer & cmp)
{
    disconnect(cmp);
}

