
#ifndef THEKLA_XMLSUBDOCUMENT_H
#define THEKLA_XMLSUBDOCUMENT_H

#include <common/config-win32.h>

#include <QString>
#include <QList>
#include <QDomDocument>

class XmlDocument;
class XmlElement;
class XmlElementData;
class Observable;
class Log;

//--------------------------------------------------------------------------------
/**
 * Light-weight container of elements linked to a root element. Does not own the
 * elements. Can be used for searching and grouping elements, passing them around.
 */
class THEKLA_EXPORT XmlSubDocument
{
private:

    friend class XmlDocument;

private:

    static Log cLog;

protected:

    XmlElement * rootElement_;

protected:

    XmlSubDocument(XmlElement * rootElement);

public:

    XmlSubDocument();

    XmlSubDocument(const XmlSubDocument & copy);

    virtual ~XmlSubDocument();

protected:

    XmlElement * setRootElement(XmlElement * rootElement);

public:

    XmlSubDocument operator=(const XmlSubDocument & rhs);

    XmlElement* getRootElement() const { return rootElement_; };

    XmlElement * findFirstElement(const XmlElementData & primaryKey) const;

    QList<XmlElement*> findElements(const XmlElementData & primaryKey) const;

    XmlElement* findFirstElement(const QString & tagName) const;

    QList<XmlElement*> findElements(const QString & tagName) const;

    XmlElement * findElement(const Observable & observable) const;

    QDomDocument getDom() const;
};

#endif // THEKLA_XMLSUBDOCUMENT_H
