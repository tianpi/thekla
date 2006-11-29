
#include "UISettings.h"
#include "XmlDatabaseDocument.h"

//--------------------------------------------------------------------------------
UISettings::UISettings()
    : XmlElement(false)
{
    xmlData_->setConfigFlag(XmlElementData::USE_THEKLA, false);
    xmlData_->setConfigFlag(XmlElementData::GENERATE_QTAPP_CODE, false);
    setServerHost("localhost");
    setServerPort(20000);
    setServerCallbackPort(20001);
    setUIDataXPathPrefix(XmlDatabaseDocument::DEFAULT_UI_DATA_XPATH_PREFIX);
}

//--------------------------------------------------------------------------------
UISettings::UISettings(const UISettings & copy)
    : XmlElement(copy)
{
}

//--------------------------------------------------------------------------------
void UISettings::init(const QDomElement & domElem)
{
    // NOTE (reimplemented from XmlElement): keep default settings by merging modified
    // attributes with default values. BUT: Do NOT clear any attribute that is not
    // contained in the given DOM element. Instead keep the default value.

    XmlElementData data(domElem);
    xmlData_->setTagName(data.getTagName());
    xmlData_->merge(data);
}
