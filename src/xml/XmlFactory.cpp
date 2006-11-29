
#include "XmlFactory.h"
#include "XmlElementInfo.h"
#include "XmlElement.h"
#include "XmlElementData.h"
#include "XmlDocument.h"
#include <common/Log.h>
#include <common/Exception.h>

// XmlElements includes
#include "UIDataRoot.h"
#include "UISettings.h"
#include "QtObject.h"
#include "QtSignal.h"
#include "QtProperty.h"
#include "OivField.h"

//--------------------------------------------------------------------------------
Log XmlFactory::cLog(Log::getClassLog("XmlFactory"));

//--------------------------------------------------------------------------------
XmlFactory::XmlFactory()
    : elementInfoList_()
{
}

//--------------------------------------------------------------------------------
XmlFactory::~XmlFactory()
{
    for (int i = 0; i< elementInfoList_.size(); i++) {
        XmlElementInfo * info = elementInfoList_[i];
        delete info;
    }
    elementInfoList_.clear();
}

//--------------------------------------------------------------------------------
void XmlFactory::registerDefaultElements()
{
    // root element
    XmlElementInfo * uiDataRootInfo = registerElement("TheklaUIData", new UIDataRoot, NULL);

    // first hierarchy level
    registerElement("Settings", new UISettings, uiDataRootInfo);
    XmlElementInfo * qtObjInfo = registerElement("QtObject", new QtObject, uiDataRootInfo);
    registerElement("OivField", new OivField, uiDataRootInfo);

    // second hierarchy level (QtObject)
    registerElement("QtProperty", new QtProperty, qtObjInfo);
    registerElement("QtSignal", new QtSignal, qtObjInfo);
}

//--------------------------------------------------------------------------------
XmlElementInfo * XmlFactory::registerElement(const QString & tagName,
                                             XmlElement * impl,
                                             XmlElementInfo * parentInfo)
{
    // setup element prototype
    if (impl != NULL) {
        impl->ownerDoc_ = dynamic_cast<XmlDocument*>(this);
        impl->getXmlData().setTagName(tagName);
    }


    // fetch existing element info
    XmlElementInfo * info = getInfo(tagName);
    if (info != NULL) {

        // modify existing element info
        info->setImpl(impl);
        info->setParentInfo(parentInfo);
    }

    else {

        // create element info
        info = new XmlElementInfo(tagName, impl, parentInfo);
        // add to list
        elementInfoList_.push_back(info);

    }

    return info;
}

//--------------------------------------------------------------------------------
void XmlFactory::unregisterElement(XmlElementInfo * elem)
{
    Log log(Log::getMethodLog(cLog, "unregisterElement()"));

    XmlElementInfo * parent = elem->getParentInfo();

    QList<XmlElementInfo*> childList = getChildInfos(elem);
    for (int i=0; i < childList.size(); i++) {
        XmlElementInfo * child = childList[i];
        child->setParentInfo(parent);
    }

    int numRemoved = -1;
    if ((numRemoved = elementInfoList_.removeAll(elem)) != 1)
        THEKLA_ERROR(log, "removeAll() returns = " << numRemoved << " (expected: 1)");

    delete elem;
}

//--------------------------------------------------------------------------------
void XmlFactory::unregisterAllElements()
{
    for (int i=0; i < elementInfoList_.size(); i++) {
        XmlElementInfo * info = elementInfoList_[i];
        delete info;
    }
    elementInfoList_.clear();
}

//--------------------------------------------------------------------------------
int XmlFactory::getElementCount() const
{
    return elementInfoList_.size();
}

//--------------------------------------------------------------------------------
XmlElementInfo * XmlFactory::getInfo(const QString & tagName) const
{
    for (int i=0; i < elementInfoList_.size(); i++) {
        XmlElementInfo * info = elementInfoList_[i];
        if (info->operator==(tagName))
            return info;
    }
    return NULL;
}

//--------------------------------------------------------------------------------
XmlElementInfo * XmlFactory::getInfo(const Observable::Type & obsType) const
{
    for (int i=0; i < elementInfoList_.size(); i++) {
        XmlElementInfo * info = elementInfoList_[i];
        if (info->operator==(obsType))
            return info;
    }
    return NULL;
}

//--------------------------------------------------------------------------------
QList<XmlElementInfo*> XmlFactory::getChildInfos(const XmlElementInfo * parentInfo)
{
    QList<XmlElementInfo*> childInfoList;
    for (int i=0; i < elementInfoList_.size(); i++) {
        XmlElementInfo * info = elementInfoList_[i];
        if (info->getParentInfo() != NULL)
            if (*(info->getParentInfo()) == *(parentInfo))
                childInfoList.push_back(info);

    }
    return childInfoList;
}

//--------------------------------------------------------------------------------
XmlElement* XmlFactory::getElement(XmlElementInfo * info) const
{
    if (info == NULL) {
//        throw Exception("");
        return NULL;
    }
    XmlElement * prototype = info->getImpl();
    if (prototype == NULL) {
        //throw Exception("");
        return NULL;
    }
    return prototype->clone();

}

//--------------------------------------------------------------------------------
XmlElement * XmlFactory::getElement(const QString & tagName) const
{
    return getElement(getInfo(tagName));
}

//--------------------------------------------------------------------------------
bool XmlFactory::isElementRegistered(const QString & tagName) const
{
    return (getInfo(tagName) != NULL);
}

//--------------------------------------------------------------------------------
XmlElement * XmlFactory::getElement(const Observable::Type & obsType) const
{
    return getElement(getInfo(obsType));
}

//--------------------------------------------------------------------------------
bool XmlFactory::isElementRegistered(const Observable::Type & obsType) const
{
    return (getInfo(obsType) != NULL);
}

