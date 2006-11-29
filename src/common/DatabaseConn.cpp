
#include "DatabaseConn.h"

#include <common/DatabaseEventDispatcher.h>
#include <common/Log.h>
#include <common/Exception.h>

#include <xml/XmlElement.h>
#include <xml/XmlElementData.h>
#include <xml/XmlDocument.h>

#include <QtCore/QTime>

using namespace MUDDLEWARE;

//--------------------------------------------------------------------------------
Log DatabaseConn::cLog(Log::getClassLog("DatabaseConn"));


//--------------------------------------------------------------------------------
DatabaseConn::DatabaseConn(const QString & serverHost,
                           int serverPort,
                           int serverCBPort)
    : mwDbConnInfo_(Info(serverHost, serverPort, serverCBPort)),
      mwDbConn_(Connection::create()),
      watchdogIdCounter(MIN_WATCHDOG_ID),
      watchdogIdToElementMap_(),
      processUpdateRequestsMutex_(),
      updateRequestsList_(),
      processWatchdogRepliesMutex_(),
      raiseEdgeOnDbChange_(NULL),
      touchOnDbChange_(NULL),
      emitSignalOnDbChange_(false)

{
    Log log(Log::getMethodLog(cLog, "DatabaseConn()"));

    //// Init Connection

    for (int i = 1; i < MAX_INIT_CONNECT_ATTEMPTS; i++) {
        if (mwDbConn_->init(serverHost.toAscii(), serverPort, serverCBPort)) {
            break;
        }
        THEKLA_WARN(log, "Cannot init connection to MW server (try no. " << i << ").");
    }

    if (!mwDbConn_->isInitialized()) {
        THROW_THEKLA_EXCEPTION("Cannot connect to MW server ("
                              << "host = " << serverHost
                              << ", port = " << serverPort
                              << ", callbackPort = " << serverCBPort
                              << ").");
    }



}

//--------------------------------------------------------------------------------
DatabaseConn::~DatabaseConn()
{
    Log log(Log::getMethodLog(cLog, "~DatabaseConn()"));
    THEKLA_DEBUG(log, "** START.");

#if 0
    // NOTE: Results in segfault together with Connection::destroy()
    if (mwDbConn_->isInitialized())
        mwDbConn_->shutdown();
#endif
    Connection::destroy(mwDbConn_);
    THEKLA_DEBUG(log, "** END.");
}

//--------------------------------------------------------------------------------
bool DatabaseConn::exists(const QString & xPath) const
{
    bool ok = mwDbConn_->getElementExists(xPath.toAscii());
    return ok; // && (ret == "true"));
}


//--------------------------------------------------------------------------------
bool DatabaseConn::exists(const XmlElement & xmlElem) const
{
    return exists(xmlElem.getXPath());
}

//--------------------------------------------------------------------------------
bool DatabaseConn::blockUntilExists(const QString & xPath, int maxBlockTimeMillis) const
{
    Log log(Log::getMethodLog(cLog, "blockUntilExists()"));
    THEKLA_WARN(log, "-- START. Blocking for XPath = " << xPath.toStdString().c_str());

    // start timer
    QTime time = QTime::currentTime();
    time.start();

    // compute milliseconds wait time
    const int DEFAULT_SLEEP_MILLIS = 500;
    int sleepMillis = DEFAULT_SLEEP_MILLIS;

    // continue to wait while element not available in DB
    while (!exists(xPath)) {

        // timer expired ..
        if (maxBlockTimeMillis >= 0 && time.elapsed() > maxBlockTimeMillis) {
            THEKLA_WARN(log, "-- END. Blocking time expired WITHOUT perceiving XPath = '"
                        << xPath.toStdString() << "'.");
            return false;
        }

        // wait ..
        log4cplus::helpers::sleepmillis(sleepMillis);
    }

    THEKLA_DEBUG(log, "-- END. Perceived XPath = " << xPath.toStdString() << ". OK.");
    return true;
}

//--------------------------------------------------------------------------------
QDomDocument DatabaseConn::select(const QString & xPath) const
{
    QueryResultsAutoPtr results = MUDDLEWARE::QueryResults::create();
    int numOfNodes = mwDbConn_->getElement(xPath.toAscii(), *results, true);

    QString xml;
    for (int i=0; i < numOfNodes; i++) {
        xml += results.get()->operator[](i).c_str();
    }

    QDomDocument doc = XmlDocument::parse(xml);

    return doc;
}

//--------------------------------------------------------------------------------
bool DatabaseConn::insert(const XmlElement & xmlElem) const
{
    Log log(Log::getMethodLog(cLog, "insert()"));

    if (xmlElem.getParent() == NULL) {
        QTextStream errMsg;
        THROW_THEKLA_EXCEPTION("Cannot derive XPath of parent XPath: "
                              << "Given element (tag = " << xmlElem.getXmlData().getTagName() << ") "
                              << "does not have a parent element.");

    }

    bool ret = mwDbConn_->addElement(xmlElem.getParent()->getXPath().toAscii(),
                                     xmlElem.getDom().toString().toAscii(),
                                     true);

    return ret;
}

//--------------------------------------------------------------------------------
int DatabaseConn::deleteFrom(const XmlElement & xmlElem) const
{
    return mwDbConn_->removeElement(xmlElem.getXPath().toAscii(), true);
}

//--------------------------------------------------------------------------------
bool DatabaseConn::update(const XmlElement & xmlElem)
{
    // Discard update request if watchdog replies are currently processed
    if (processWatchdogRepliesMutex_.isLocked()) {
        return true;
    }

    // Store Element on update request list for later processing
    if (!processUpdateRequestsMutex_.isLocked()) {
        cacheUpdateRequest(const_cast<XmlElement*>(&xmlElem));
        return true;
    }

    // NOTE: The entire hierarchy of elements is updated NOT only the root element.

    return mwDbConn_->updateElement(xmlElem.getXPath().toAscii(),
                                    xmlElem.getDom().toString().toAscii(),
                                    true);
}

//--------------------------------------------------------------------------------
bool DatabaseConn::update(const QList<XmlElement *> & xmlElemList)
{
    // Discard update request if watchdog replies are currently processed
    if (processWatchdogRepliesMutex_.isLocked()) {
        return true;
    }

    // Store Element on update request list for later processing
    if (!processUpdateRequestsMutex_.isLocked()) {
        for (int i = 0; i < xmlElemList.size(); i++) {
            XmlElement * xmlElem = xmlElemList[i];
            cacheUpdateRequest(xmlElem);
        }
        return true;
    }

    Request * request = Request::create();
    Reply * reply = Reply::create();

    for (int i = 0; i < xmlElemList.size(); i++) {
        const XmlElement * xmlElem = xmlElemList[i];
        if (xmlElem == NULL) continue;
        request->updateElement(xmlElem->getXPath().toAscii(),
                               xmlElem->getDom().toString().toAscii(),
                               true);
    }

    bool ret = mwDbConn_->sendAndReceive(*request, *reply);

    Request::destroy(request);
    Reply::destroy(reply);

    return ret;
}

//--------------------------------------------------------------------------------
int DatabaseConn::registerWatchdog(XmlElement & xmlElement)
{
    Log log(Log::getMethodLog(cLog, "registerWatchdog()"));
    THEKLA_DEBUG(log, "** START.");

    // Return actual id and do as if nothing has happened ;-)
    if (xmlElement.hasWatchdog()) {

        if (watchdogIdToElementMap_.key(&xmlElement) != xmlElement.getWatchdogId())
        {
            THEKLA_ERROR(log, "Given Element (" << xmlElement.getXPath().toStdString() << ") "
                         << "is NOT registered on watchdog element list "
                         << "(watchdogId = " << xmlElement.getWatchdogId() << "). "
                         << "BUT: Element claims to HAVE a Watchdog.");
        }

        return xmlElement.getWatchdogId();
    }

    // Given element is already registered -> Error
    if (watchdogIdToElementMap_.keys(&xmlElement).size() != 0) {
        THEKLA_ERROR(log, "Given Element (" << xmlElement.getXPath().toStdString() << ") "
                     << "IS registered on watchdog element list. "
                     << "BUT: Element claims to NOT have a Watchdog.");
    }

    const int watchdogId = watchdogIdCounter++;

    // NOTE: Connection::registerWatchDog() always returns true ??

    // NOTE: Currently synchronous watchdogs cannot be used, because of the following
    // scenario: Lets assume, two clients have registered watchdogs to the same
    // Element. The first one updates the Element. What SHOULD happen is, that the second
    // client sends an empty request within its next watchdog processing cycle. What
    // ACTUALLY happens is, that the first client is faster and SHOULD receive the
    // watchdog answer in question. But ACTUALLY it doesn't: the Reply is not sent because
    // of the "noSelf == true" flag (absolutely correct). Ergo: It seems as in the current
    // implementation of Muddleware EXACTLY ONE client can catch the Response. According
    // to my understanding, it SHOULD be that way, that ANY client which registered a sync
    // watchdog should be able to catch the Response (TODO: talk to Daniel about that
    // matter).

    if (!mwDbConn_->registerWatchDog(xmlElement.getXPath().toAscii(),
                                     watchdogId,
                                     true, // async == false per default // ((watchdogId % 2) ? true : false),
                                     true, // noself == false per default
                                     true)) // noDoubles == false per default
    {
        THROW_THEKLA_EXCEPTION("Registering watchdog on XPath "
                               << "(" << xmlElement.getXPath() << ") "
                               << "failed for unknown reason.");
    }

    //// register element

    watchdogIdToElementMap_.insert(watchdogId, &xmlElement);

    return watchdogId;
}

//--------------------------------------------------------------------------------
void DatabaseConn::unregisterWatchdog(XmlElement & xmlElement)
{
    Log log(Log::getMethodLog(cLog, "unregisterWatchdog()"));
    THEKLA_DEBUG(log, "** START.");

    const int watchdogId = xmlElement.getWatchdogId();

    // Return silently
    if (!xmlElement.hasWatchdog()) {

        if (watchdogIdToElementMap_.key(&xmlElement) != watchdogId) {
            THEKLA_ERROR(log, "Given Element (" << xmlElement.getXPath().toStdString() << ") "
                         << "IS registered on watchdog list. "
                         << "(watchdogId = " << xmlElement.getWatchdogId() << "). "
                         << "BUT: Element claims to NOT have a watchdog.");
        }

        return;
    }

    if (watchdogIdToElementMap_.keys(&xmlElement).size() != 1 ||
        watchdogIdToElementMap_.key(&xmlElement) != watchdogId)
    {
        QList<int> watchdogIdList = watchdogIdToElementMap_.keys(&xmlElement);
        THEKLA_WARN(log, "watchdogIdList.size() = " << watchdogIdList.size());
        for (int m = 0; m < watchdogIdList.size(); m++) {
            THEKLA_WARN(log, "watchdogIdList[ " << m << "] = " << watchdogIdList[m]);
        }
        THEKLA_ERROR(log, "Given Element (" << xmlElement.getXPath().toStdString() << ") "
                     << "is NOT registed on watchdog list. "
                     << "(watchdogId = " << xmlElement.getWatchdogId() << "). "
                     << "BUT: Element claims to HAVE a watchdog.");
    }

    // remove the watchdog
    if (!mwDbConn_->unregisterWatchDog(watchdogId)) {
        THEKLA_ERROR(log, "sendAndReceive() failed (returned false).");
    }


    // remove from list
    int removedValuesCount = watchdogIdToElementMap_.remove(watchdogId);
    if (removedValuesCount != 1)
    {
        THEKLA_ERROR(cLog, "Removed other than ONE VALUE (" << removedValuesCount << ")"
                     << " given KEY (" << watchdogId << ").");
    }

}


//--------------------------------------------------------------------------------
int DatabaseConn::processWatchdogReplies()
{
    Log log(Log::getMethodLog(cLog, "processWatchdogReplies()"));
    THEKLA_DEBUG(log, "** START.");

    //// Acquire mutex

    if (!processWatchdogRepliesMutex_.tryLock()) {
        THEKLA_DEBUG(log, "** END. Mutex locked.");
        return 0;
    }

    //// Retrieve pending Replies and sort (newer Replies first)

    typedef QList<Reply*> ReplyList;
    ReplyList replyList;
    Reply * reply = NULL;

#if 0 // currently only async Watchdogs supported

    // Retrieve all pending SYNC Watchdog replies by sending an empty request

    {
        reply = Reply::create();
        Request * request = Request::create();
        request->emptyRequest();

        if (!mwDbConn_->sendAndReceive(*request, *reply)) {
            THEKLA_WARN(log, "** END. sendAndReceive() returned false");
            processWatchdogRepliesMutex_.unlock();
            return 0;
        }

        THEKLA_DEBUG(log, "reply = " << reply);

        replyList.push_front(reply);

        Request::destroy(request);
    }

#endif

    // Retrieve all pending ASYNC Watchdog replies

    // NOTE: Tests show, that replies are delivered in the order of their arrival (FIFO),
    // that means: oldest first. We store them in a list where newest are first.


    while ((reply = mwDbConn_->getAsynchWatchdogResult()) != NULL) {

        replyList.push_front(reply);

    }

    THEKLA_DEBUG(log, "replyList.size() = " << replyList.size());

    if (replyList.size() == 0) {
        THEKLA_DEBUG(log, "Reply count equals ZERO.");
        processWatchdogRepliesMutex_.unlock();
        return 0;
    }

    ////  Notifiy connected listeners about change in LOCAL (!) DB

    notifyOnDbChange();

    //// Process Reply list (newest Reply first!)

    QMap<QString, QDomDocument> xPathToDomMap;
    QList<int> clientIdList;
    int totalAnswerCount = 0;

    for (ReplyList::ConstIterator it = replyList.begin(); it != replyList.end(); it++) {

        reply = *it;

        for (int i = 0; i < reply->getNumAnswers(); i++) {

            totalAnswerCount++;

            // process answer
            const Answer * answer = reply->getAnswerByIndex(i);

            // retrieve data from answer
            const QString xPath(answer->getXPath());
            const int clientId(answer->getClientData());

            THEKLA_DEBUG(log, "++ <Answer>");

            THEKLA_DEBUG(log, "Answer::getXPath() = " << xPath.toStdString());
            THEKLA_DEBUG(log, "Answer::getClientData() = " << clientId);

            //// Filter clientId (= watchdogId): process each id only once

            if (clientIdList.contains(clientId)) {
                THEKLA_DEBUG(log, "-- </Answer>: totalAnswerCount = " << totalAnswerCount
                            << ", already processed clientId = " << clientId);
                continue;
            }

            //// Retrieve XMLElement which is registered under current clientId

#if 0
            // NOTE: temporary testing (!)
            int numXmlElems = watchdogIdToElementMap_.values(clientId).size();
            if (numXmlElems != 1) {
                THEKLA_ERROR(log, "Not exactly one (" << numXmlElems << ") "
                            << "XmlElement registered for clientId (" << clientId << ").");

                processWatchdogRepliesMutex_.unlock();
                return totalAnswerCount;
            }
#endif

            // Mapping constraint: watchdogId -> (exactly ONE) Element
            XmlElement* xmlElem = watchdogIdToElementMap_.value(clientId);
            clientIdList.push_back(clientId);

            THEKLA_DEBUG(log, "XmlElement::getWatchdogId() = " << xmlElem->getWatchdogId());

            //// Parse XML: use Cache to store parsed XML documents

            // NOTE: Parse XML iff the given element contains text (and thus must be a
            // leaf element implizitely). Introduced during the "onDbChange" issue where
            // we are just interested in the OCCURENCE of the watchdog event, not in the
            // element VALUE (text) change

//             if (xmlElem->isLeaf) {
            if (xmlElem->getXmlData().containsText()) {

                QDomDocument doc;

                // XML document of current XPath is contained in cache

                if (!xPathToDomMap.contains(xPath)) {

                    // Compose XML string from Answer
                    QString xml;
                    QTextStream xmlStream(&xml, QIODevice::WriteOnly);
                    for (int i = 0; i < answer->getNumItems(); i++) {
                        xmlStream << QString(answer->getItem(i));
                    }

                    THEKLA_DEBUG(log, "(!) Parsing XML (" << xml.toStdString() << ").");

                    // Parse XML text to XML document
                    doc = XmlDocument::parse(xml);

                    // Insert into cache
                    xPathToDomMap.insert(xPath, doc);

                }

                // XML document of current XPath is ALREA contained in cache

                else {

                    doc = xPathToDomMap.value(xPath);

                }

                //// Merge Element with new XML data

                xmlElem->merge(doc.documentElement());


            } // if containsText()

            //// Sychronize with Observers

            xmlElem->sync();

            THEKLA_DEBUG(log, "-- </Answer>: totalAnswerCount = " << totalAnswerCount);

        }

        Reply::destroy(reply);
    }

    //// Release mutex

    processWatchdogRepliesMutex_.unlock();

    THEKLA_DEBUG(log, "** END. totalAnswerCount = " << totalAnswerCount);
    return totalAnswerCount;
}

//--------------------------------------------------------------------------------
bool DatabaseConn::operator==(const DatabaseConn::Info & cmp)
{
    return (mwDbConnInfo_.serverHost_ == cmp.serverHost_ &&
            mwDbConnInfo_.serverPort_ == cmp.serverPort_ &&
            mwDbConnInfo_.serverCBPort_ == cmp.serverCBPort_);
}

//--------------------------------------------------------------------------------
int DatabaseConn::processUpdateRequests()
{
    Log log(Log::getMethodLog(cLog, "processUpdateRequests()"));
    THEKLA_DEBUG(log, "** START. updateRequestsList_.size() = " << updateRequestsList_.size());

    // Acquire the lock: otherwise the update() methods cache the Elements
    if (!processUpdateRequestsMutex_.tryLock()) {
        THEKLA_DEBUG(log, "** END. mutex is LOCKED.");
        return 0;
    }

    bool updateRequestsCount = updateRequestsList_.size();

    // Perform the update database operations: process Elements that were cached
    if (!update(updateRequestsList_)) {
        THEKLA_WARN(log, "update() returned FALSE");
    }
    // all updates requests processed -> clear list
    updateRequestsList_.clear();

    // Release the lock -> update() methods cache incoming Elements
    processUpdateRequestsMutex_.unlock();

    THEKLA_DEBUG(log, "** END. updateRequestsList_.size() = " << updateRequestsList_.size());
    return updateRequestsCount;
}

//--------------------------------------------------------------------------------
void DatabaseConn::cacheUpdateRequest(XmlElement * xmlElem)
{
    Log log(Log::getMethodLog(cLog, "cacheUpdateRequest()"));
    THEKLA_DEBUG(log, "** START. updateRequestsList_.size() = " << updateRequestsList_.size());

    // Remove all (old) list items of given Element
    updateRequestsList_.removeAll(xmlElem);
    // Add Element at top of list (policy: newer on top)
    updateRequestsList_.push_front(xmlElem);

    THEKLA_DEBUG(log, "** END. updateRequestsList_.size() = " << updateRequestsList_.size());
}


//--------------------------------------------------------------------------------
bool DatabaseConn::event(QEvent * event)
{
    Log log(Log::getMethodLog(cLog, "event()"));
    THEKLA_DEBUG(log, "** START. event.type() = " << event->type());

    //// Catch AsyncEvent

    if ((AsyncEvent::Type)event->type() == AsyncEvent::WATCHDOG_REPLY) {

        processWatchdogReplies();

        THEKLA_DEBUG(log, "** END. AsyncEvent!! Consumed.");
        return true;
    }

    THEKLA_DEBUG(log, "** END. Some other event. Not consumed.");
    return false;
}

//--------------------------------------------------------------------------------
void DatabaseConn::notifyOnDbChange()
{
    Log log(Log::getMethodLog(cLog, "notifyOnDbChange()"));

    THEKLA_DEBUG(log, "** START.");

    // SoSFBool: transition "false" -> "true"

    if (raiseEdgeOnDbChange_ != NULL) {
        if (((bool)raiseEdgeOnDbChange_->getValue()) == false)
            raiseEdgeOnDbChange_->setValue(true);
    }

    // SoSFTrigger: u can't touch this ;-)

    if (touchOnDbChange_ != NULL) {
        touchOnDbChange_->touch();
    }

    // emit Signal

    if (emitSignalOnDbChange_) {
        emit notifyOnDbChangeSignal();
    }

    THEKLA_DEBUG(log, "** END.");
}

//--------------------------------------------------------------------------------
void DatabaseConn::notifyOnDbChange(SoSFBool * oivField)
{
    raiseEdgeOnDbChange_ = oivField;
    if (raiseEdgeOnDbChange_ != NULL)
        raiseEdgeOnDbChange_->setValue(false);
}

//--------------------------------------------------------------------------------
void DatabaseConn::notifyOnDbChange(QObject * target, const char * slotSignature)
{
    Log log(Log::getMethodLog(cLog, "notifyOnDbChange()"));
    THEKLA_DEBUG(log, "** START. slotSignature = " << slotSignature);

    if (connect(this, SIGNAL ( notifyOnDbChangeSignal() ), target, slotSignature)) {
        emitSignalOnDbChange_ = true;
    }

    THEKLA_DEBUG(log, "** END. emitSignalOnDbChange_ = " << emitSignalOnDbChange_);
}
