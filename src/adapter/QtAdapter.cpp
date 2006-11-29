
#include "QtAdapter.h"
#include <xml/QtObject.h>
#include <common/Log.h>

//--------------------------------------------------------------------------------
Log QtAdapter::cLog(Log::getClassLog("QtAdapter"));

//--------------------------------------------------------------------------------
bool QtAdapter::signalEmit(const QString & signalSignature, const QByteArray & paramData)
{
    Q_UNUSED(signalSignature);
    Q_UNUSED(paramData);
    return false;
}

//--------------------------------------------------------------------------------
void QtAdapter::signalChanged(const QString & signalSignature, const QByteArray & paramData)
{
    Log log(Log::getMethodLog(cLog, "signalChanged()"));
    THEKLA_DEBUG(log, "** START. signalSignature = " << signalSignature.toStdString()
                << ", paramData.size() = " << paramData.size());
    owner_->updateSignal(signalSignature, paramData);
    THEKLA_DEBUG(log, "** END.");
}

//--------------------------------------------------------------------------------
void QtAdapter::propertyUpdateTrigger()
{
    Log log(Log::getMethodLog(cLog, "propertyUpdateTrigger()"));
    THEKLA_DEBUG(log, "** START.");
    owner_->updateProperties();
    THEKLA_DEBUG(log, "** END.");
}
