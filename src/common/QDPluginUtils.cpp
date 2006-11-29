
#include "QDPluginUtils.h"
#include "Log.h"

#include <sys/types.h>
#include <sys/stat.h>

//--------------------------------------------------------------------------------
Log QDPluginUtils::cLog(Log::getClassLog("QDPluginUtils"));

//--------------------------------------------------------------------------------
QString QDPluginUtils::getConfigFilePath(const QString & uiFileName)
{
    QString ret;
    int indexLastDot = uiFileName.lastIndexOf('.');
    ret = uiFileName.left(indexLastDot);
    ret += "-thekla.xml";
    return ret;
}

//--------------------------------------------------------------------------------
QString QDPluginUtils::getUiWrapperFilePath(const QString & uiFilePath)
{
    QString uiFileName = getFileName(uiFilePath);

    QString ret;
    int indexLastDot = uiFileName.lastIndexOf('.');
    if (indexLastDot == -1)
        indexLastDot = uiFileName.size();

    ret = getPathName(uiFilePath);
    ret += "thekla_";
    ret += uiFileName.left(indexLastDot);
    ret += ".h";
    return ret;
}

//--------------------------------------------------------------------------------
QString QDPluginUtils::getUiHeaderFilePath(const QString & uiFilePath)
{
    QString uiFileName = getFileName(uiFilePath);

    QString ret;
    int indexLastDot = uiFileName.lastIndexOf('.');
    if (indexLastDot == -1)
        indexLastDot = uiFileName.size();

    ret = getPathName(uiFilePath);
    ret += "ui_";
    ret += uiFileName.left(indexLastDot);
    ret += ".h";
    return ret;
}

//--------------------------------------------------------------------------------
QString QDPluginUtils::getQtAppProjectFilePath(const QString & uiFilePath)
{
    QString uiFileName = getFileName(uiFilePath);

    QString ret;
    int indexLastDot = uiFileName.lastIndexOf('.');
    if (indexLastDot == -1)
        indexLastDot = uiFileName.size();

    ret = getPathName(uiFilePath);
    ret += uiFileName.left(indexLastDot);
    ret += ".pro";
    return ret;
}

//--------------------------------------------------------------------------------
QString QDPluginUtils::getQtAppMainFilePath(const QString & uiFilePath)
{
    QString uiFileName = getFileName(uiFilePath);

    QString ret;
    int indexLastDot = uiFileName.lastIndexOf('.');
    if (indexLastDot == -1)
        indexLastDot = uiFileName.size();

    ret = getPathName(uiFilePath);
    ret += "main.cpp";
    return ret;
}


//--------------------------------------------------------------------------------
QString QDPluginUtils::getFileName(const QString & filePath)
{
    QString ret;
    int indexLastSlash = std::max(filePath.lastIndexOf('/'), filePath.lastIndexOf('\\')) + 1;
    ret = filePath.right(filePath.size() - indexLastSlash);
    return ret;
}

//--------------------------------------------------------------------------------
QString QDPluginUtils::getPathName(const QString & filePath)
{
    QString ret;
    int indexLastSlash = std::max(filePath.lastIndexOf('/'), filePath.lastIndexOf('\\')) + 1;
    ret = filePath.left(indexLastSlash);
    return ret;
}

//--------------------------------------------------------------------------------
time_t QDPluginUtils::getModifyTime(const QString & filePath)
{
    // ::stat : can be used equally on UNIX and WIN32 (see HW filestat.h::load())
    struct stat buf;
    if (::stat(filePath.toStdString().c_str(), &buf) != 0)
        return -1;

    //time_t    st_mtime;   /* time of last modification */
    return buf.st_mtime;
}

