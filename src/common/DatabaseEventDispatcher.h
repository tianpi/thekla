#ifndef THEKLA_DATABASEEVENTDISPATCHER_H
#define THEKLA_DATABASEEVENTDISPATCHER_H

#include <Muddleware/Client/AsyncWatchdogListener.h>

#include <QtCore/QObject>
#include <QtCore/QEvent>
#include <QtCore/QThread>
#include <QtCore/QList>

class Log;
class DatabaseConn;

//--------------------------------------------------------------------------------
class AsyncEvent : public QEvent
{
public:
    enum Type {
        WATCHDOG_REPLY = QEvent::User
    };

public:
    AsyncEvent(Type type)
        : QEvent((QEvent::Type)type)
        {};
    ~AsyncEvent()
        {};
};


//--------------------------------------------------------------------------------
class DatabaseEventDispatcher
{

private:

    typedef void (DatabaseEventDispatcher::*CallbackMethod) (DatabaseConn * target); // pointer on member

    //--------------------------------------------------------------------------------
    class Timer : public QThread
    {

    private:

        int timeout_;

        bool stop_;

        DatabaseEventDispatcher * targetObj_;

        DatabaseEventDispatcher::CallbackMethod callbackMethod_;

    public:

        Timer();

        ~Timer();

    protected:

        void run();

        void start();

    public:

        void init(DatabaseEventDispatcher * targetObj,
                  DatabaseEventDispatcher::CallbackMethod callbackMethod);

        void start(unsigned int timeout);

        void stop();

        bool isActive() const
            { return isRunning(); };

        int interval() const
            { return timeout_; };
    };

    //--------------------------------------------------------------------------------
    class AsyncWatchdogListener : public MUDDLEWARE::AsyncWatchdogListener
    {
    private:

        static Log cLog;

    private:

        DatabaseEventDispatcher * dbEventDispatcher_;

        DatabaseConn * dbConn_;

    public:

        AsyncWatchdogListener(DatabaseEventDispatcher * dbEventDispatcher, DatabaseConn * dbConn);

        ~AsyncWatchdogListener() {};

    private:

        void AWL_OnNewAsyncWatchdog();

    };


private:

    static Log cLog;

public:

    static DatabaseEventDispatcher * qtAppEventDispatcher;

    static DatabaseEventDispatcher * nonQtAppEventDispatcher;

public:

    static const int DEFAULT_PROCESS_UPDATE_REQUESTS_TIMEOUT;

    static const int DEFAULT_PROCESS_WATCHDOG_REPLIES_TIMEOUT;

public:

    static void init();

private:

    QList<DatabaseConn *> targetDbConnList_;

    QList<AsyncWatchdogListener *> asyncWatchdogListenerList_;

    bool useQtEventLoop_;

    Timer processUpdateRequestsTimer_;

    Timer processWatchdogRepliesTimer_;

    bool enableUpdateRequestsProcessing_;

    bool enableWatchdogRepliesProcessing_;

private:

    DatabaseEventDispatcher();

    DatabaseEventDispatcher(const DatabaseEventDispatcher & copy);

    DatabaseEventDispatcher(bool useQtEventLoop);

    ~DatabaseEventDispatcher();

private:

    void setTimeoutAux(Timer * timer, unsigned int timeout);

    void triggerProcessUpdateRequests(DatabaseConn * target = NULL);

    void triggerProcessWatchdogReplies(DatabaseConn * target = NULL);

public:

    void setProcessUpdateRequestsTimeout(unsigned int timeout);

    void setProcessWatchdogRepliesTimeout(unsigned int timeout);

    void enableUpdateRequestsProcessing(bool enable)
        { enableUpdateRequestsProcessing_ = enable; };
    bool isUpdateRequestsProcessingEnabled() const
        { return enableUpdateRequestsProcessing_; };

    void enableWatchdogRepliesProcessing(bool enable)
        { enableWatchdogRepliesProcessing_ = enable; };
    bool isWatchdogRepliesProcessingEnabled() const
        { return enableWatchdogRepliesProcessing_; };

    void addTargetDbConn(DatabaseConn * targetDbConn);

    void removeTargetDbConn(DatabaseConn * targetDbConn);

};

#endif // THEKLA_DATABASEEVENTDISPATCHER_H
