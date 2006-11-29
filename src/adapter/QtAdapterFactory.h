
#ifndef THEKLA_QTADAPTERFACTORY_H
#define THEKLA_QTADAPTERFACTORY_H

#include <common/config-win32.h>

#include "QtAdapter.h"
#include <QMap>
#include <QString>

#define QTADAPTERCLASSLIST_QMAKEPRJFILE "QtAdapterClassList.pri"
#define QTADAPTERCLASSLIST_INCLUDEFILE "QtAdapterClassList.h"
#define QTADAPTERCLASSLIST_MACROFILE "QtAdapterClassList.cpp"

class QtObject;

//--------------------------------------------------------------------------------
class THEKLA_EXPORT QtAdapterFactory
{
private:

    typedef struct {
        QtAdapter::CreatorFunc creatorFunc_;
    } QtAdapterInfo;

public:
    typedef QMap<QString, QtAdapterInfo> AdapterMap;

private:

    static AdapterMap defaultAdapterMap_;

    static bool isDAMInitialized_;

private:

    static void initDefaultAdapterMap();

private:

    AdapterMap customAdapterMap_;

public:

    QtAdapterFactory();

    ~QtAdapterFactory() {};

public:

    QtAdapter* getAdapter(const QString & qObjClassName, QtObject * adapterOwner);

    QtAdapter* getAdapter(const QObject * qObj, QtObject * adapterOwner);

    bool existsAdapter(const QString & qObjClassName);

    void addCustomAdapter(const QString & qObjClassName, QtAdapter::CreatorFunc creatorFunc);

};

#endif // THEKLA_QTADAPTERFACTORY_H
