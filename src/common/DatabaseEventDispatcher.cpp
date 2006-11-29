#include "DatabaseEventDispatcher.h"

#include <common/DatabaseConn.h>
#include <common/Log.h>

#include <QtGui/QApplication>


//================================================================================

//--------------------------------------------------------------------------------
DatabaseEventDispatcher::Timer::Timer()
    : QThread(),
      timeout_(0),
      stop_(true),
      targetObj_(NULL),
      callbackMethod_(NULL)
{
}

//--------------------------------------------------------------------------------
DatabaseEventDispatcher::Timer::~Timer()
{
    if (isRunning()) stop();
}

//--------------------------------------------------------------------------------
void DatabaseEventDispatcher::Timer::init(DatabaseEventDispatcher * targetObj,
                                          DatabaseEventDispatcher::CallbackMethod callbackMethod)
{
    targetObj_ = targetObj;
    callbackMethod_ = callbackMethod;
}

//--------------------------------------------------------------------------------
void DatabaseEventDispatcher::Timer::run()
{
    while (!stop_) {
        msleep(timeout_);
        (targetObj_->*callbackMethod_)(NULL);
    }
}

//--------------------------------------------------------------------------------
void DatabaseEventDispatcher::Timer::start()
{
    stop_ = false;
    QThread::start(QThread::InheritPriority);
}

//--------------------------------------------------------------------------------
void DatabaseEventDispatcher::Timer::start(unsigned int timeout)
{
    if (isRunning()) {
        stop();
    }
    if (timeout > 0) {
        timeout_ = timeout;
        start();
    }
}

//--------------------------------------------------------------------------------
void DatabaseEventDispatcher::Timer::stop()
{
    stop_ = true;
    wait();
};

//================================================================================

//--------------------------------------------------------------------------------
Log DatabaseEventDispatcher::AsyncWatchdogListener::cLog(Log::getClassLog("AsyncWatchdogListener"));

//--------------------------------------------------------------------------------
DatabaseEventDispatcher::AsyncWatchdogListener::AsyncWatchdogListener(
    DatabaseEventDispatcher * dbEventDispatcher, DatabaseConn * dbConn)
    : dbEventDispatcher_(dbEventDispatcher),
      dbConn_(dbConn)
{
}

//--------------------------------------------------------------------------------
void DatabaseEventDispatcher::AsyncWatchdogListener::AWL_OnNewAsyncWatchdog()
{
    Log log(Log::getMethodLog(cLog, "AWL_OnNewAsyncWatchdog()"));
    THEKLA_DEBUG(log, "** START. processWatchdogRepliesTimer_.isActive() = "
                 << dbEventDispatcher_->processWatchdogRepliesTimer_.isActive());

    // Policy: Either (1) the timer is running or (2) send an event to ourselves that is
    // processed within the Qt event thread

    if (dbEventDispatcher_->processWatchdogRepliesTimer_.isActive())
        return;

    // NOTE (thread savety): Several QWidget classes do NOT tolerate when their properties
    // are updated asynchronously (eg QTextList with property "html")

    // NOTE: the Qt event thread is started by calling the Q(Core)Application::exec(). Qt
    // requires EXACTLY one instance of QApplication (singleton - see also "qApp" macro
    // and QApplication::instance() static method). Non-Qt-Applications may use Thekla
    // without creating a QApplication object (and thus MUST NOT use QWidgets et al). The
    // distinction done here is: Either (1) the singleton QApplication instance exists
    // (and was implizitely created OUTSIDE Thekla): then it is assumed that the event
    // thread is also executed. Or (2) QApplication is NULL and therefore the main event
    // loop is definitely not running.

    // NOTE: The Event that is posted here, is destroyed by Qt. It can be catched by
    // overwriting the QObject:event() method within the target class (this).

    dbEventDispatcher_->triggerProcessWatchdogReplies(dbConn_);
}

//================================================================================


//--------------------------------------------------------------------------------
Log DatabaseEventDispatcher::cLog(Log::getClassLog("DatabaseEventDispatcher"));

const int DatabaseEventDispatcher::DEFAULT_PROCESS_UPDATE_REQUESTS_TIMEOUT = 0;
const int DatabaseEventDispatcher::DEFAULT_PROCESS_WATCHDOG_REPLIES_TIMEOUT = 0;

DatabaseEventDispatcher * DatabaseEventDispatcher::qtAppEventDispatcher = NULL;
DatabaseEventDispatcher * DatabaseEventDispatcher::nonQtAppEventDispatcher = NULL;


//--------------------------------------------------------------------------------
void DatabaseEventDispatcher::init()
{
    Log log(Log::getMethodLog(cLog, "init()"));
    THEKLA_DEBUG(log, "** START.");

    if (qtAppEventDispatcher == NULL)
        qtAppEventDispatcher = new DatabaseEventDispatcher(true);

    if (nonQtAppEventDispatcher == NULL)
        nonQtAppEventDispatcher = new DatabaseEventDispatcher(false);
}


//--------------------------------------------------------------------------------
DatabaseEventDispatcher::DatabaseEventDispatcher(bool useQtEventLoop)
    : targetDbConnList_(),
      asyncWatchdogListenerList_(),
      useQtEventLoop_(useQtEventLoop),
      processUpdateRequestsTimer_(),
      processWatchdogRepliesTimer_(),
      enableUpdateRequestsProcessing_(true),
      enableWatchdogRepliesProcessing_(true)
{
    // initialize update requests processing timer
    processUpdateRequestsTimer_.init(this, &DatabaseEventDispatcher::triggerProcessUpdateRequests);
    setProcessUpdateRequestsTimeout(DEFAULT_PROCESS_UPDATE_REQUESTS_TIMEOUT);

    // initialize watchdog replies processing timer
    processWatchdogRepliesTimer_.init(this, &DatabaseEventDispatcher::triggerProcessWatchdogReplies);
    setProcessWatchdogRepliesTimeout(DEFAULT_PROCESS_WATCHDOG_REPLIES_TIMEOUT);
}

//--------------------------------------------------------------------------------
DatabaseEventDispatcher::~DatabaseEventDispatcher()
{
    for (int i = 0; i < asyncWatchdogListenerList_.size(); i++) {
        targetDbConnList_[i]->mwDbConn_->setAsyncWatchdogListener(NULL);
        delete asyncWatchdogListenerList_[i];
    }
    targetDbConnList_.clear();
    asyncWatchdogListenerList_.clear();
}

//--------------------------------------------------------------------------------
/**
 * Restarts the timer that triggers the processing of update requests with given timeout
 * interval. If the given timeout equals zero, the timer is removed. With no timer
 * running, the update requests are processed immediately (fastest but most CPU-resource
 * intensive).
 *
 * @param timeout timer interval in milliseconds (recommended: >= 20)
 */
void DatabaseEventDispatcher::setProcessUpdateRequestsTimeout(unsigned int timeout)
{
    setTimeoutAux(&processUpdateRequestsTimer_, timeout);

    if (timeout == 0) {
        for (int i = 0; i < targetDbConnList_.size(); i++) {
            targetDbConnList_[i]->processUpdateRequestsMutex_.lock();
        }
    } else {
        for (int i = 0; i < targetDbConnList_.size(); i++) {
            if (targetDbConnList_[i]->processUpdateRequestsMutex_.isLocked())
                targetDbConnList_[i]->processUpdateRequestsMutex_.unlock();
        }
    }

 }

//--------------------------------------------------------------------------------
/**
 * Restarts the timer that triggers the processing of watchdog replies with given timeout
 * interval. If the given timeout equals zero, the timer is removed and the watchdog reply
 * processing is triggered asynchronously (fastest).
 *
 * @param timeout timer interval in milliseconds (recommended: >= 20)
 */
void DatabaseEventDispatcher::setProcessWatchdogRepliesTimeout(unsigned int timeout)
{
    setTimeoutAux(&processWatchdogRepliesTimer_, timeout);
}

//--------------------------------------------------------------------------------
void DatabaseEventDispatcher::setTimeoutAux(Timer * timer, unsigned int timeout)
{
    if (timer->isActive())
        timer->stop();

    if (timeout > 0)
        timer->start(timeout);

}

//--------------------------------------------------------------------------------
void DatabaseEventDispatcher::triggerProcessUpdateRequests(DatabaseConn * target)
{
    if (!enableUpdateRequestsProcessing_)
        return;

    if (target != NULL) {
        target->processUpdateRequests();
    } else {
        for (int i = 0; i < targetDbConnList_.size(); i++) {
            targetDbConnList_[i]->processUpdateRequests();
        }
    }
}


//--------------------------------------------------------------------------------
void DatabaseEventDispatcher::triggerProcessWatchdogReplies(DatabaseConn * target)
{
    if (!enableWatchdogRepliesProcessing_)
        return;

    if (useQtEventLoop_) {

        if (target != NULL) {
            QApplication::postEvent(target, new AsyncEvent(AsyncEvent::WATCHDOG_REPLY));
        } else {
            for (int i = 0; i < targetDbConnList_.size(); i++) {
                QApplication::postEvent(targetDbConnList_[i], new AsyncEvent(AsyncEvent::WATCHDOG_REPLY));
            }
        }
    } else {
        if (target != NULL) {
            target->processWatchdogReplies();
        } else {
            for (int i = 0; i < targetDbConnList_.size(); i++) {
                targetDbConnList_[i]->processWatchdogReplies();
            }
        }
    }
}

//--------------------------------------------------------------------------------
void DatabaseEventDispatcher::addTargetDbConn(DatabaseConn * targetDbConn)
{
    if (targetDbConnList_.contains(targetDbConn))
        return;

    AsyncWatchdogListener * wl = new AsyncWatchdogListener(this, targetDbConn);
    targetDbConn->mwDbConn_->setAsyncWatchdogListener(wl);

    targetDbConnList_.push_front(targetDbConn);
    asyncWatchdogListenerList_.push_front(wl);

    if (!processUpdateRequestsTimer_.isActive()) {
        targetDbConn->processUpdateRequestsMutex_.lock();
    }

}

//--------------------------------------------------------------------------------
void DatabaseEventDispatcher::removeTargetDbConn(DatabaseConn * targetDbConn)
{
    Log log(Log::getMethodLog(cLog, "DatabaseEventDispatcher.removeTargetDbConn()"));
    THEKLA_DEBUG(log, "** START. targetDbConnList_.size() = " << targetDbConnList_.size());

    if (!targetDbConnList_.contains(targetDbConn))
        return;

    int i = -1;
    while ((i = targetDbConnList_.indexOf(targetDbConn)) != -1) {
        targetDbConnList_.removeAt(i);
        asyncWatchdogListenerList_.removeAt(i);
    }

    targetDbConn->mwDbConn_->setAsyncWatchdogListener(NULL);

    if (targetDbConn->processUpdateRequestsMutex_.isLocked()) {
        targetDbConn->processUpdateRequestsMutex_.unlock();
    }

    THEKLA_DEBUG(log, "** END. targetDbConnList_.size() = " << targetDbConnList_.size());
}


