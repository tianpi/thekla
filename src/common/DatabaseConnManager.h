
#ifndef THEKLA_DATABASECONNMANAGER_H
#define THEKLA_DATABASECONNMANAGER_H

#include "DatabaseConn.h"
#include <QList>

class DatabaseConnInfo;

//--------------------------------------------------------------------------------
class DatabaseConnManager
{
private:

    static Log cLog;

    static QList<DatabaseConnInfo> dbConnList_;

private:

    DatabaseConnManager();

    ~DatabaseConnManager();

public:

    static DatabaseConn * acquire(const DatabaseConn::Info & dbConnInfo);

    static void release(DatabaseConn *& dbConn);

    static int getUserCount(DatabaseConn * dbConn);

    static int getConnCount();

};

#endif // THEKLA_DATABASECONNMANAGER_H
