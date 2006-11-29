
#ifndef THEKLA_XMLELEMENT_H
#define THEKLA_XMLELEMENT_H

#include <common/config-win32.h>
#include <common/Observable.h>
#include <common/Observer.h>

#include <QList>
#include <QString>
#include <QDomElement>
#include <QDomDocument>

class DatabaseConn;
class XmlElementData;
class Observable;
class XmlDocument;
class Log;


//--------------------------------------------------------------------------------
class THEKLA_EXPORT XmlElement
{
private:

    friend class XmlFactory;

public:

    enum State {
        BOUND_TO_OBSERVABLE = 0x01
    };

private:

    static Log cLog;

protected:

    unsigned int state_;

    XmlElementData * xmlData_;

    XmlDocument * ownerDoc_;

    XmlElement * parent_;

    QList<XmlElement*> children_;

    const Observable * observable_;

    QList<const Observer *> observerList_;

    int watchdogId_;

protected:

    XmlElement(const XmlElement & copy);

public:

    explicit XmlElement(bool containsText);

    virtual ~XmlElement();

protected:

    void connectBefore(const Observer * observer);

    void connectAfter(const Observer * observer);

    void bindBefore();

    void bindAfter();

    void dyingObservable();

    void dyingObserver(const Observer & cmp);

public:

    virtual XmlElement* clone() const;

    bool changeOwnerDocument(XmlDocument * ownerDoc, bool applyRecursive = true);

    virtual void init(const QDomElement & domElem);

    virtual void init(const Observable * observable);

    virtual bool canInit(Observable::Type obsType) const { Q_UNUSED(obsType); return false; };

    bool hasObservable() const { return observable_ != NULL; };

    bool operator==(const Observable & cmp) const;

    const Observable * getObservable() const { return observable_; };

    const QList<const Observer*> & getObservers() const { return observerList_; };

    virtual XmlElement* setParent(XmlElement * parent);

    XmlElement* getParent() const { return parent_; };

    int getChildCount() const { return children_.size(); };

    XmlElement* getChild(int index) const { return children_.value(index); };

    virtual void bind();

    virtual void unbind() { };

    virtual void connect(const Observer * observer);

    virtual void disconnect(const Observer & cmp) { Q_UNUSED(cmp); };

    QString getXPath() const;

    void insertIntoDB() const;

    virtual void update();

    virtual void merge(const Observable * observable, bool syncObservableFirst = false);

    virtual void merge(const QDomElement & domElem);

    virtual void merge(const XmlElementData & xmlData);

    virtual void merge(const DatabaseConn & dbConn);

    virtual void sync() const { };

    virtual void syncObservable() { };

    QDomDocument getDom() const;

    XmlElement* findFirstElement(const QString & tagName);

    QList<XmlElement*> findElements(const QString & tagName);

    XmlElement* findFirstElement(const XmlElementData & primaryKey);

    QList<XmlElement*> findElements(const XmlElementData & primaryKey);

    XmlElement* findElement(const Observable & observable);

    inline XmlElementData & getXmlData() const { return *xmlData_; };

    inline XmlDocument* getOwnerDocument() const { return ownerDoc_; };

    DatabaseConn* getDatabaseConn() const;

    void updateSetDB() const;

    void updateSetDB(const QList<XmlElement *> & xmlElemList) const;

    void registerWatchdog();

    void unregisterWatchdog();

    bool hasWatchdog() const;

    int getWatchdogId() const { return watchdogId_; };

    bool isLeaf() const;

    void enableState(State flag, bool enable);

    bool isStateEnabled(State flag) const { return (state_ & flag); };

};

#endif // THEKLA_XMLELEMENT_H
