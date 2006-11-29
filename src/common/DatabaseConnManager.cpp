
#include "DatabaseConnManager.h"

#include <common/Log.h>

//--------------------------------------------------------------------------------
class DatabaseConnInfo {

public:

    DatabaseConn* dbConn_;

    int clientCounter_;

private:

    DatabaseConnInfo();

public:

    DatabaseConnInfo(DatabaseConn * dbConn)
        : dbConn_(dbConn),
          clientCounter_(1)
        {};

    ~DatabaseConnInfo()
        {};
};


//--------------------------------------------------------------------------------
Log DatabaseConnManager::cLog(Log::getClassLog("DatabaseConnManager"));

//--------------------------------------------------------------------------------
QList<DatabaseConnInfo> DatabaseConnManager::dbConnList_;

//--------------------------------------------------------------------------------
DatabaseConn * DatabaseConnManager::acquire(const DatabaseConn::Info & dbConnInfo)
{
    Log log(Log::getMethodLog(cLog, "acquire()"));
    THEKLA_DEBUG(log, "** START. host = " << dbConnInfo.serverHost_.toStdString()
                << ", port = " << dbConnInfo.serverPort_
                << ", cb-port = " << dbConnInfo.serverCBPort_);

    DatabaseConn * ret = NULL;

    // return existing connection
    bool useExisting = false;
    for (int i = 0; i < dbConnList_.size(); i++) {

        DatabaseConnInfo & cmp = dbConnList_[i];
        if (cmp.dbConn_->operator==(dbConnInfo)) {
            ret = cmp.dbConn_;
            cmp.clientCounter_++;
            useExisting = true;
            break;
        }
    }

    // create new connection
    if (!useExisting) {

        ret = new DatabaseConn(dbConnInfo.serverHost_, dbConnInfo.serverPort_, dbConnInfo.serverCBPort_);
        dbConnList_.push_back(DatabaseConnInfo(ret));

    }

    if (ret == NULL) {
        THEKLA_ERROR(log, "Returned DatabaseConn MUST NOT be NULL.");
    }

    THEKLA_DEBUG(log, "** END. ret = " << ret);
    return ret;
}

//--------------------------------------------------------------------------------
void DatabaseConnManager::release(DatabaseConn *& dbConn)
{
    if (dbConn == NULL)
        return;

    // find connection within list
    for (int i = 0; i < dbConnList_.size(); i++) {

        DatabaseConnInfo & cmp = dbConnList_[i];

        if (cmp.dbConn_ == dbConn) {

            // decrement counter
            cmp.clientCounter_--;

            // no users then delete the connection
            if (cmp.clientCounter_ == 0) {
                dbConnList_.removeAt(i);
                delete dbConn;
            }

            dbConn = NULL;

            return;
        }
    }

}

//--------------------------------------------------------------------------------
int DatabaseConnManager::getUserCount(DatabaseConn * dbConn)
{
    for (int i = 0; i < dbConnList_.size(); i++) {
        DatabaseConnInfo & cmp = dbConnList_[i];
        if (cmp.dbConn_ == dbConn) {
            return cmp.clientCounter_;
        }
    }

    return 0;
}

//--------------------------------------------------------------------------------
int DatabaseConnManager::getConnCount()
{
    return dbConnList_.size();
}
