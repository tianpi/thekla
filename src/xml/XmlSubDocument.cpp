
#include "XmlSubDocument.h"
#include "XmlElement.h"
#include "XmlElementData.h"
#include <common/Log.h>

//--------------------------------------------------------------------------------
Log XmlSubDocument::cLog(Log::getClassLog("XmlSubDocument"));

//--------------------------------------------------------------------------------
XmlSubDocument::XmlSubDocument()
    : rootElement_(NULL)
{
}

//--------------------------------------------------------------------------------
XmlSubDocument::XmlSubDocument(XmlElement * rootElement)
    : rootElement_(rootElement)
{
}

//--------------------------------------------------------------------------------
XmlSubDocument::XmlSubDocument(const XmlSubDocument & copy)
    : rootElement_(copy.rootElement_)

{
}

//--------------------------------------------------------------------------------
XmlSubDocument::~XmlSubDocument()
{
}

//--------------------------------------------------------------------------------
XmlSubDocument XmlSubDocument::operator=(const XmlSubDocument & rhs)
{
    if (this != &rhs) {
        rootElement_ = rhs.rootElement_;
    }
    return *this;
}


//--------------------------------------------------------------------------------
XmlElement* XmlSubDocument::findFirstElement(const QString & tagName) const
{
    if (rootElement_ == NULL)
        return NULL;
    return (rootElement_->findFirstElement(tagName));
}

//--------------------------------------------------------------------------------
XmlElement * XmlSubDocument::findFirstElement(const XmlElementData & primaryKey) const
{
    Log log(Log::getMethodLog(cLog, "findFirstElement(XmlElementData)", log4cplus::OFF_LOG_LEVEL));
    THEKLA_DEBUG(log, "** START. primaryKey.getAttributeCount() = " << primaryKey.getAttributeCount());
    if (rootElement_ == NULL) {
        THEKLA_DEBUG(log, "** END. rootElement == NULL");
        return NULL;
    }
    THEKLA_DEBUG(log, "** END. rootElement = " << rootElement_);
    XmlElement * ret = rootElement_->findFirstElement(primaryKey);
    THEKLA_DEBUG(log, "** END. ret = " << ret);
    return ret;
}

//--------------------------------------------------------------------------------
QList<XmlElement*> XmlSubDocument::findElements(const XmlElementData & primaryKey) const
{
    if (rootElement_ == NULL)
        return QList<XmlElement*>();
    return (rootElement_->findElements(primaryKey));
}


//--------------------------------------------------------------------------------
QList<XmlElement*> XmlSubDocument::findElements(const QString & tagName) const
{
    if (rootElement_ == NULL)
        return QList<XmlElement*>();
    return (rootElement_->findElements(tagName));
}

//--------------------------------------------------------------------------------
XmlElement * XmlSubDocument::findElement(const Observable & observable) const
{
    if (rootElement_ == NULL)
        return NULL;
    return (rootElement_->findElement(observable));
}


//--------------------------------------------------------------------------------
XmlElement * XmlSubDocument::setRootElement(XmlElement * rootElement)
{
    XmlElement * prev = NULL;
    prev = rootElement_;
    rootElement_ = rootElement;
    return prev;
}

//--------------------------------------------------------------------------------
QDomDocument XmlSubDocument::getDom() const
{
    if (rootElement_ == NULL)
        return QDomDocument();
    return rootElement_->getDom();
}

