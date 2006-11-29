
#include "QDFormWindowUiFileObserver.h"
#include <common/Log.h>
#include <common/QDPluginUtils.h>
#include <QFile>

//--------------------------------------------------------------------------------
Log QDFormWindowUiFileObserver::cLog(Log::getClassLog("QDFormWindowUiFileObserver", log4cplus::OFF_LOG_LEVEL));


//--------------------------------------------------------------------------------
QDFormWindowUiFileObserver::QDFormWindowUiFileObserver(QObject * parent)
    : QObject(parent),
      uiFileName_(""),
      uiFileModifyTimePrev_(0),
      timer_()
{
    Log log(Log::getMethodLog(cLog, "QDFormWindowUiFileObserver()"));
    THEKLA_DEBUG(log, "** START. parent = " << parent);
    THEKLA_DEBUG(log, "** END. timer.isActive() = " << timer_.isActive());
}

//--------------------------------------------------------------------------------
void QDFormWindowUiFileObserver::start(const QString & uiFileName)
{
    Log log(Log::getMethodLog(cLog, "start()"));
    THEKLA_DEBUG(log, "** START. uiFileName = " << uiFileName.toStdString());

    // timer is running -> do nothing
    if (timer_.isActive()) {
        THEKLA_DEBUG(log, "** END. Timer is active.");
        return;
    }

    // save ui file name
    setUiFileName(uiFileName);
    // NO initial signal
    uiFileModifyTimePrev_ = QDPluginUtils::getModifyTime(uiFileName);

    // connect and start timer
    connect(&timer_, SIGNAL(timeout()), this, SLOT(checkUiFile()));
    timer_.start(DEFAULT_TIMER_INTERVAL_MILLIS);

    THEKLA_DEBUG(log, "** END. timer.isActive() = " << timer_.isActive());
}

//--------------------------------------------------------------------------------
QDFormWindowUiFileObserver::~QDFormWindowUiFileObserver()
{
    if (timer_.isActive()) timer_.stop();
}

//--------------------------------------------------------------------------------
void QDFormWindowUiFileObserver::setUiFileName(const QString & uiFileName)
{
    Log log(Log::getMethodLog(cLog, "setUiFileName()"));
    THEKLA_DEBUG(log, "** START. uiFileName = " << uiFileName.toStdString());

    if (uiFileName == uiFileName_)
        return;

    // store given file name
    uiFileName_ = uiFileName;
    // 0 : trigger a guaranteed signal
    uiFileModifyTimePrev_ = 0; // getModifyTime(uiFileName);

    THEKLA_DEBUG(log, "** END. uiFileModifyTimePrev_ = " << uiFileModifyTimePrev_);
}


//--------------------------------------------------------------------------------
void QDFormWindowUiFileObserver::checkUiFile()
{
    Log log(Log::getMethodLog(cLog, "checkUiFile()"));
    THEKLA_DEBUG(log, "** START. uiFileModifyTimePrev_ = " << uiFileModifyTimePrev_);

    // File not valid -> do nothing
    if (!QFile::exists(uiFileName_)) {
        return;
    }

    //// compare UI file modification time

    time_t uiFileModifyTime = QDPluginUtils::getModifyTime(uiFileName_);
    THEKLA_DEBUG(log, "** START. uiFileModifyTime= " << uiFileModifyTime);
    if (uiFileModifyTime > uiFileModifyTimePrev_) {

        // save modification time
        uiFileModifyTimePrev_ = uiFileModifyTime;

        THEKLA_DEBUG(log, "-- Emit Signal uiFileModified()");

        // emit signal
        emit uiFileModified(uiFileName_);
    }

    THEKLA_DEBUG(log, "** END. uiFileModifyTimePrev_ = " << uiFileModifyTimePrev_);

}
