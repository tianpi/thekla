
#ifndef THEKLA_THEKLAROOTELEMENT_H
#define THEKLA_THEKLAROOTELEMENT_H

#include "XmlElement.h"
#include "XmlElementData.h"

class Log;

//--------------------------------------------------------------------------------
class THEKLA_EXPORT UIDataRoot : public XmlElement
{
private:
    static Log cLog;

protected:

    UIDataRoot(const UIDataRoot & copy);

public:

    UIDataRoot();

    virtual ~UIDataRoot();

public:

    virtual XmlElement* clone() const { return new UIDataRoot(*this); };

    void setAppId(const QString & appId)
        { getXmlData().setAttribute("app-id", appId); };
    QString getAppId() const
        { return xmlData_->getAttribute("app-id"); };

};

#endif // THEKLA_THEKLAROOTELEMENT_H
