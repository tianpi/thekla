
#ifndef THEKLA_THEKLASETTINGS_H
#define THEKLA_THEKLASETTINGS_H

#include "XmlElement.h"
#include "XmlElementData.h"

//--------------------------------------------------------------------------------
class THEKLA_EXPORT UISettings : public XmlElement
{
protected:
    UISettings(const UISettings & copy);

public:
    UISettings();
    virtual ~UISettings()
        {};

public:

    virtual XmlElement* clone() const { return new UISettings(*this); };

    virtual void init(const QDomElement & domElem);

    // GETers and SETers of the attribute values. SETers because the settings are editet
    // within the QDesigner plugin.

    void setServerHost(const QString & serverHost)
        { xmlData_->setAttribute("server-host", serverHost); };
    QString getServerHost() const
        { return xmlData_->getAttribute("server-host"); }

    void setServerPort(int serverPort)
        { xmlData_->setAttribute("server-port", QString::number(serverPort)); };
    int getServerPort() const
        { return xmlData_->getAttribute("server-port").toInt(); }

    void setServerCallbackPort(int serverCallbackPort)
        { xmlData_->setAttribute("server-callback-port", QString::number(serverCallbackPort)); };
    int getServerCallbackPort() const
        { return xmlData_->getAttribute("server-callback-port").toInt(); }

    void setUiFileName(const QString & uiFileName)
        { getXmlData().setAttribute("ui-file-name", uiFileName); };
    QString getUiFileName() const
        { return xmlData_->getAttribute("ui-file-name"); };

    void setMainContainerType(const QString & mainContType)
        { getXmlData().setAttribute("main-container-type", mainContType); };
    QString getMainContainerType() const
        { return xmlData_->getAttribute("main-container-type"); };

    void setUIDataXPathPrefix(const QString & uiDataXPathPrefix)
        { getXmlData().setAttribute("ui-data-xpath-prefix", uiDataXPathPrefix); };
    QString getUIDataXPathPrefix() const
        { return xmlData_->getAttribute("ui-data-xpath-prefix"); };


};

#endif // THEKLA_THEKLASETTINGS_H
