
#include "QDWidget.h"

#include <common/Log.h>

//--------------------------------------------------------------------------------
Log QDWidget::cLog(Log::getClassLog("QDPlugin"));

//--------------------------------------------------------------------------------
QDWidget::QDWidget(QWidget *parent )
    : QWidget(parent)
{
    Log log(Log::getMethodLog(cLog, "QDWidget()"));
    THEKLA_DEBUG(log, "** START");
    THEKLA_DEBUG(log, "** END");
}
