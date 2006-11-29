
#ifndef THEKLA_QDPLUGINUTILS_H
#define THEKLA_QDPLUGINUTILS_H

#include <common/config-win32.h>
#include <QString>

class Log;

//--------------------------------------------------------------------------------
class THEKLA_EXPORT QDPluginUtils
{
private:
    static Log cLog;

public:

    static QString getConfigFilePath(const QString & uiFilePath);

    static QString getUiHeaderFilePath(const QString & uiFilePath);

    static QString getUiWrapperFilePath(const QString & uiFilePath);

    static QString getQtAppMainFilePath(const QString & uiFilePath);

    static QString getQtAppProjectFilePath(const QString & uiFileName);

    static QString getFileName(const QString & filePath);

    static QString getPathName(const QString & filePath);

    static time_t getModifyTime(const QString & filePath);


private:
    QDPluginUtils();
    ~QDPluginUtils();

};


#endif // THEKLA_QDPLUGINUTILS_H
