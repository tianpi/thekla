
#ifndef THEKLA_DATABASECONN_H
#define THEKLA_DATABASECONN_H

#include <common/config-win32.h>

#include <Muddleware/Client/Connection.h>

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QMutex>
#include <QtCore/QEvent>
#include <QtXml/QDomDocument>

#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFTrigger.h>

class XmlElement;
class Log;
class DatabaseEventDispatcher;

//--------------------------------------------------------------------------------
class THEKLA_EXPORT DatabaseConn : public QObject
{

    Q_OBJECT

private:

    friend class DatabaseEventDispatcher;

    //--------------------------------------------------------------------------------
    class Mutex : public QMutex
    {
    public:

        Mutex(QMutex::RecursionMode mode = QMutex::NonRecursive)
            : QMutex(mode)
            {};
        ~Mutex()
            {};

    public:

        bool isLocked()
            {
                if (!tryLock()) return true;
                unlock();
                return false;
            };
    };

public:

    //--------------------------------------------------------------------------------
    class Info {

    public:
        QString serverHost_;
        int serverPort_;
        int serverCBPort_;

    private:
        Info();

    public:
        Info(QString serverHost, int serverPort, int serverCBPort)
            : serverHost_(serverHost),
              serverPort_(serverPort),
              serverCBPort_(serverCBPort)
            {};
        ~Info()
            {};
    };


private:

    static Log cLog;

    static const int MAX_INIT_CONNECT_ATTEMPTS = 5;

public:

    static const int MIN_WATCHDOG_ID = 1;

    static const int DEFAULT_BLOCK_TIME_MILLIS = 5000;

    static const int INFINITE_BLOCK_TIME_MILLIS = -1;

private:

    Info mwDbConnInfo_;

    MUDDLEWARE::Connection * mwDbConn_;

    int watchdogIdCounter;

    QMap<int, XmlElement*> watchdogIdToElementMap_;

    Mutex processUpdateRequestsMutex_;
    QList<XmlElement*> updateRequestsList_;

    Mutex processWatchdogRepliesMutex_;

    SoSFBool * raiseEdgeOnDbChange_;
    SoSFTrigger * touchOnDbChange_;
    bool emitSignalOnDbChange_;

private:

    DatabaseConn();

    DatabaseConn(const DatabaseConn & copy);

    DatabaseConn operator=(const DatabaseConn & rhs);

public:

    DatabaseConn(const QString & serverHost, int serverPort, int serverCBPort);

    virtual ~DatabaseConn();

private:

    void cacheUpdateRequest(XmlElement * xmlElem);

    void notifyOnDbChange();

public:

    bool operator==(const DatabaseConn::Info & cmp);

    bool insert(const XmlElement & xmlElem) const;

    bool update(const XmlElement & xmlElem);
    bool update(const QList<XmlElement *> & xmlElemList);

    bool exists(const XmlElement & xmlElem) const;
    bool exists(const QString & xPath) const;

    bool blockUntilExists(const QString & xPath,
                          int maxBlockTimeMillis = DEFAULT_BLOCK_TIME_MILLIS) const;

    int deleteFrom(const XmlElement & xmlElem) const;

    QDomDocument select(const QString & xPath) const;

    int registerWatchdog(XmlElement & xmlElement);

    void unregisterWatchdog(XmlElement & xmlElement);

    int processUpdateRequests();

    int processWatchdogReplies();

    virtual bool event(QEvent * event);

    const DatabaseConn::Info * getInfo() const
        { return &mwDbConnInfo_; };

    void notifyOnDbChange(SoSFBool * oivField);

    void notifyOnDbChange(SoSFTrigger * oivField)
        { touchOnDbChange_ = oivField; };

    void notifyOnDbChange(QObject * target, const char * slotSignature);


signals:

    void notifyOnDbChangeSignal();
};

#endif // THEKLA_DATABASECONN_H
