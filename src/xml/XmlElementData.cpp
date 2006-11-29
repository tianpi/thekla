
#include "XmlElementData.h"

#include <common/Base64.h>
#include <common/Log.h>

//================================================================================

//--------------------------------------------------------------------------------
typedef struct {
    XmlElementData::ConfigFlag flag_;
    QString attribName_;
} ConfigFlagToAttributeMapping;

//--------------------------------------------------------------------------------
static ConfigFlagToAttributeMapping configFlagToAttribNameMap_[] = {
    { XmlElementData::USE_THEKLA, "use-thekla" },
    { XmlElementData::GENERATE_QTAPP_CODE, "generate-qtapp-code" },
    { XmlElementData::BIND, "bind" },
    { XmlElementData::USE_AS_TRIGGER, "use-as-trigger" },
    { XmlElementData::UPDATE, "update" },
    { XmlElementData::CF_NONE, "" },
};


//================================================================================

//--------------------------------------------------------------------------------
Log XmlElementData::cLog(Log::getClassLog("XmlElementData", log4cplus::OFF_LOG_LEVEL));

//--------------------------------------------------------------------------------
XmlElementData::XmlElementData(bool containsText)
    : doc_(""),
      data_(doc_.createElement("must-not-be-empty")),
      containsText_(containsText),
      text_(doc_.createTextNode(""))
{
    Log log(Log::getMethodLog(cLog, "XmlElementData()"));
    THEKLA_DEBUG(log, "-- START");
    if (containsText_) data_.appendChild(text_);
    doc_.appendChild(data_);
    THEKLA_DEBUG(log, "-- END");
}

//--------------------------------------------------------------------------------
XmlElementData::XmlElementData(const XmlElementData & copy)
    : doc_(copy.doc_.cloneNode(true).toDocument()),
      data_(doc_.documentElement()),
      containsText_(copy.containsText_),
      text_()
{
    Log log(Log::getMethodLog(cLog, "XmlElementData(XmlElementData&)"));
    THEKLA_DEBUG(log, "** START");

    if (containsText_) text_ = data_.firstChild().toText();

    THEKLA_DEBUG(log, "** END");
}

//--------------------------------------------------------------------------------
XmlElementData::~XmlElementData()
{

}


//--------------------------------------------------------------------------------
XmlElementData & XmlElementData::operator=(const XmlElementData & rightData)
{
    Log log(Log::getMethodLog(cLog, "operator=()"));
    THEKLA_DEBUG(log, "** START.");

    if (this != &rightData) {

        doc_.clear();
        doc_ = rightData.doc_.cloneNode(true).toDocument();
        data_ = doc_.documentElement();
        containsText_ = rightData.containsText_;
        if (containsText_)
            text_ = data_.firstChild().toText();
        else
            text_.clear();

    }

    THEKLA_DEBUG(log, "** END.");
    return *this;
}


//--------------------------------------------------------------------------------
XmlElementData::XmlElementData(const QDomElement & domElem)
    : doc_(""),
      data_(domElem.cloneNode(false).toElement()),
      containsText_(false),
      text_()
{
    doc_.appendChild(data_);

    QDomNodeList children = domElem.childNodes();
    for (int i = 0; i < children.count(); i++) {
        QDomNode child = children.item(i);
        if (child.isText()) {
            containsText_ = true;
            text_ = child.cloneNode(false).toText();
            data_.appendChild(text_);
            break;
        }
    }
}

//--------------------------------------------------------------------------------
bool XmlElementData::operator==(const XmlElementData & cmp)
{
    if (getTagName() != cmp.getTagName())
        return false;
    if (getAttributeCount() != cmp.getAttributeCount())
        return false;
    for (int i=0; i < getAttributeCount(); i++) {
        QString attribName = getAttributeName(i);
        if (!cmp.hasAttribute(attribName))
            return false;
        if (getAttribute(attribName) != cmp.getAttribute(attribName))
            return false;
    }
    if (containsText() != cmp.containsText())
        return false;
    if (containsText())
        if (getText() != cmp.getText())
            return false;
    return true;
}

//--------------------------------------------------------------------------------
bool XmlElementData::contains(const XmlElementData & cmp)
{
    if (getTagName() != cmp.getTagName())
        return false;
    if (getAttributeCount() < cmp.getAttributeCount())
        return false;
    for (int i=0; i < cmp.getAttributeCount(); i++) {
        QString attribName = cmp.getAttributeName(i);
        if (!hasAttribute(attribName))
            return false;
        if (getAttribute(attribName) != cmp.getAttribute(attribName))
            return false;
    }
    if (cmp.containsText())
        if (getText() != cmp.getText())
            return false;
    return true;
}


//--------------------------------------------------------------------------------
int XmlElementData::getAttributeCount() const
{
    QDomNamedNodeMap attribs = data_.attributes();
    return attribs.count();
}

//--------------------------------------------------------------------------------
QString XmlElementData::getAttributeName(int index) const
{
    QDomNamedNodeMap attribs = data_.attributes();
    QDomAttr attrib = attribs.item(index).toAttr();
    return attrib.name();
}

//--------------------------------------------------------------------------------
QString XmlElementData::getAttributeValue(int index) const
{
    QDomNamedNodeMap attribs = data_.attributes();
    QDomAttr attrib = attribs.item(index).toAttr();
    return attrib.value();
}

//--------------------------------------------------------------------------------
void XmlElementData::setText(const QByteArray & text, bool encodeBase64)
{
    if (!containsText_)
        return;
    QByteArray textSave = text;
    if (encodeBase64) {
        textSave = Base64::encode(text);
    }
    text_.setData(textSave);
}

//--------------------------------------------------------------------------------
QByteArray XmlElementData::getText(bool decodeBase64) const
{
    QByteArray ret;
    if (!containsText_) return ret;

    ret = text_.data().toAscii();
    if (decodeBase64)
        ret = Base64::decode(ret);
    return ret;
}

//--------------------------------------------------------------------------------
QDomDocument XmlElementData::getDom() const
{
    return doc_.cloneNode(true).toDocument();
}

//--------------------------------------------------------------------------------
QString XmlElementData::getXPath() const
{
    Log log(Log::getMethodLog(cLog, "getXPath()"));
    THEKLA_DEBUG(log, "-- START.");
    QString xPath;
    xPath += "/";
    xPath += getTagName();
    int attribCount = getAttributeCount();
    for (int i = 0; i < attribCount; i++) {
        QString attribName(getAttributeName(i));
        QString attribValue(getAttributeValue(i));
        if (isConfigFlag(attribName))
            continue;
        xPath += "[@" + attribName + "='" + attribValue + "'" + "]";
    }
    THEKLA_DEBUG(log, "-- END. xPath = " << xPath.toStdString());
    return xPath;
}


//--------------------------------------------------------------------------------
void XmlElementData::setConfigFlag(ConfigFlag flag, bool value)
{
    for (int i = 0; configFlagToAttribNameMap_[i].flag_ != CF_NONE; i++) {
        ConfigFlagToAttributeMapping mapping = configFlagToAttribNameMap_[i];
        if (mapping.flag_ == flag) {
            setAttribute(mapping.attribName_, (value) ? "true" : "false");
        }
    }
}

//--------------------------------------------------------------------------------
bool XmlElementData::getConfigFlag(ConfigFlag flag) const
{
    for (int i = 0; configFlagToAttribNameMap_[i].flag_ != CF_NONE; i++) {
        ConfigFlagToAttributeMapping mapping = configFlagToAttribNameMap_[i];
        if (mapping.flag_ == flag) {
            if (!hasAttribute(mapping.attribName_))
                return false;
            return (getAttribute(mapping.attribName_) == "true") ? true : false;
        }
    }
    return false;
}

//--------------------------------------------------------------------------------
bool XmlElementData::isConfigFlag(const QString & attribName) const
{
    for (int i = 0; configFlagToAttribNameMap_[i].flag_ != CF_NONE; i++) {
        ConfigFlagToAttributeMapping mapping = configFlagToAttribNameMap_[i];
        if (mapping.attribName_ == attribName) {
            return true;
        }
    }
    return false;
}

//--------------------------------------------------------------------------------
void XmlElementData::merge(const XmlElementData & data)
{
    // merge attributes
    for (int i = 0; i < data.getAttributeCount(); i++) {
        setAttribute(data.getAttributeName(i), data.getAttributeValue(i));
    }

    // mismatching text containers
    if (containsText_ != data.containsText_)
        return;

    // set text
    if (containsText_) {
        setText(data.getText());
    }

}

//--------------------------------------------------------------------------------
void XmlElementData::clear()
{
    // remove Attributes
    while (getAttributeCount() > 0) {
        removeAttribute(getAttributeName(0));
    }

    // reset text
    if (containsText_) {
        setText("");
    }

    // reset tag name
    setTagName("");
}

