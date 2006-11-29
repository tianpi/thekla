
#include "XmlElementInfo.h"
#include "XmlElement.h"

//--------------------------------------------------------------------------------
XmlElementInfo::XmlElementInfo(const QString & tagName, XmlElement * impl, XmlElementInfo * parentInfo)
    : tagName_(tagName),
      impl_(impl),
      parentInfo_(parentInfo)
{
}

//--------------------------------------------------------------------------------
XmlElementInfo::~XmlElementInfo()
{
    if (impl_ != NULL) delete impl_;
}

//--------------------------------------------------------------------------------
bool XmlElementInfo::operator==(const QString & tagName) const
{
    return (tagName_ == tagName);
}

//--------------------------------------------------------------------------------
bool XmlElementInfo::operator==(const Observable::Type & obsType) const
{
    if (impl_ == NULL) return false;
    return (impl_->canInit(obsType));
}

//--------------------------------------------------------------------------------
bool XmlElementInfo::operator==(const XmlElementInfo & cmp) const
{
    if (tagName_ != cmp.tagName_)
        return false;
    if (parentInfo_ != cmp.parentInfo_)
        return false;
    if (parentInfo_ == NULL)
        return true;
    return (parentInfo_->operator==(*(cmp.parentInfo_)));
}

//--------------------------------------------------------------------------------
XmlElementInfo * XmlElementInfo::setParentInfo(XmlElementInfo * parentInfo)
{
    XmlElementInfo * prev = parentInfo_;
    parentInfo_ = parentInfo;
    return prev;
}

//--------------------------------------------------------------------------------
void XmlElementInfo::setImpl(XmlElement * impl)
{
    XmlElement * prev = impl_;
    impl_ = impl;
    if (prev != NULL) delete prev;
}

