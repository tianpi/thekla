
#include "UIDataRoot.h"

#include <common/Log.h>
#include <common/Exception.h>

//--------------------------------------------------------------------------------
Log UIDataRoot::cLog(Log::getClassLog("UIDataRoot"));

//--------------------------------------------------------------------------------
UIDataRoot::UIDataRoot()
    : XmlElement(false)
{
    setAppId("");
}

//--------------------------------------------------------------------------------
UIDataRoot::UIDataRoot(const UIDataRoot & copy)
    : XmlElement(copy)
{

}

//--------------------------------------------------------------------------------
UIDataRoot::~UIDataRoot()
{

}

