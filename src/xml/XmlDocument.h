
#ifndef THEKLA_XMLDOCUMENT_H
#define THEKLA_XMLDOCUMENT_H

#include "XmlSubDocument.h"
#include "XmlFactory.h"

#include <QFile>

class XmlFactory;
class XmlSubDocument;
class DatabaseConn;
class QtAdapterFactory;

//--------------------------------------------------------------------------------
class THEKLA_EXPORT XmlDocument : public XmlSubDocument, public XmlFactory
{
private:

    static Log cLog;

public:

    static QDomDocument parse(const QString & xmlText);

    static QDomDocument parse(QFile & file);

protected:

    QtAdapterFactory * adapterFactory_;
    bool isInitialized_;

private:

    XmlDocument(const XmlDocument & copy);

protected:

    XmlDocument();
    virtual ~XmlDocument();

private:

    XmlDocument operator=(const XmlDocument & rhs);

    XmlElement* createSubDocument(const QDomElement & domElem);

    XmlSubDocument createSubDocument(QObject * qObject);

    XmlElement * mergeSubDocument(QObject * qObj, bool syncObservableFirst = false);

    XmlElement* cloneSubDocumentAux(XmlElement * xmlElem);

public:

    QtAdapterFactory * getAdapterFactory() { return adapterFactory_; };

    virtual XmlElement* addElement(XmlElement * xmlElem);

    virtual XmlElement* removeElement(XmlElement * xmlElem);

    XmlElement * findElementExtended(const Observable & observable) const;

    XmlSubDocument createSubDocument(XmlElement * rootElement);

    XmlSubDocument createSubDocument(const QDomDocument & domDoc);

    XmlSubDocument createSubDocument(const Observable & observable);

    XmlSubDocument cloneSubDocument(XmlElement * rootElement);

    XmlElement * mergeSubDocument(const Observable & observable, bool syncObservableFirst = false);

    void mergeDocument(QObject * qObjMainContainer, bool syncObservableFirst = false);

    void updateDocument();

    void updateSubDocument(XmlElement * xmlElem);

    void syncObservableSubDocument(XmlElement * xmlElem);

};

#endif // THEKLA_XMLDOCUMENT_H
