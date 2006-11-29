
#ifndef THEKLA_XMLFACTORY_H
#define THEKLA_XMLFACTORY_H

#include <common/config-win32.h>
#include <common/Observable.h>

#include <QMap>
#include <QString>
#include <QDomElement>

class XmlElement;
class XmlDocument;
class XmlElementInfo;
class Log;

//--------------------------------------------------------------------------------
class THEKLA_EXPORT XmlFactory
{
private:

    static Log cLog;

protected:

    QList<XmlElementInfo *> elementInfoList_;

protected:

    XmlFactory();

    virtual ~XmlFactory();

protected:

    void unregisterAllElements();

    void unregisterElement(XmlElementInfo * elem);

    void registerDefaultElements();

    XmlElementInfo * registerElement(const QString & tagName,
                                     XmlElement * impl,
                                     XmlElementInfo * parentInfo);

    XmlElement* getElement(XmlElementInfo * info) const;

    int getElementCount() const;

public:

    XmlElementInfo * getInfo(const QString & tagName) const;

    XmlElementInfo * getInfo(const Observable::Type & obsType) const;

    QList<XmlElementInfo*> getChildInfos(const XmlElementInfo * parentInfo);

    XmlElement * getElement(const QString & tagName) const;
    bool isElementRegistered(const QString & tagName) const;

    XmlElement * getElement(const Observable::Type & obsType) const;
    bool isElementRegistered(const Observable::Type & obsType) const;

};

#endif // THEKLA_XMLFACTORY_H

