
#ifndef THEKLA_XMLDATABASEDOCUMENT_H
#define THEKLA_XMLDATABASEDOCUMENT_H

#include "XmlDocument.h"

class DatabaseConn;
class XmlConfigFileDocument;
class UIDataRoot;

//--------------------------------------------------------------------------------
class THEKLA_EXPORT XmlDatabaseDocument : public XmlDocument
{
private:

    static Log cLog;

public:

    static const QString DEFAULT_UI_DATA_XPATH_PREFIX;

protected:

    DatabaseConn & dbConn_;

private:

    XmlDatabaseDocument();

protected:

    void mergeWithDbRecursive(XmlElement & xmlElem);

    void insertIntoDbRecursive(const XmlElement & xmlElem);

    void insertIntoOrUpdateDbRecursive(XmlElement & xmlElem);

    void insertIntoOrMergeDbRecursive(XmlElement & xmlElem);

public:

    XmlDatabaseDocument(DatabaseConn & dbConn);

    virtual ~XmlDatabaseDocument();

public:

    virtual void init(const QString & appId,
                      const QString & uiDataRootXPathPrefix = DEFAULT_UI_DATA_XPATH_PREFIX);

    void init(const XmlConfigFileDocument & fileDoc, bool mergeSubDocWithDb = false);

    void applyConfiguration();

    XmlElement* addElement(XmlElement * xmlElem, bool mergeSubDocWithDb);

    virtual XmlElement* addElement(XmlElement * xmlElem);

    DatabaseConn & getDatabaseConn() { return dbConn_; }

    UIDataRoot* getUIDataRoot() const;

};


typedef XmlDatabaseDocument XmlDatabaseInputDocument;


typedef XmlDatabaseDocument XmlDatabaseOutputDocument;


#endif // XMLDATABASEDOCUMENT_H
