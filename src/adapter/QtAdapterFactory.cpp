
#include "QtAdapterFactory.h"
#include "DefaultAdapterGenerator.h"

#include QTADAPTERCLASSLIST_INCLUDEFILE

//--------------------------------------------------------------------------------
bool QtAdapterFactory::isDAMInitialized_ = false;
QtAdapterFactory::AdapterMap QtAdapterFactory::defaultAdapterMap_ = QtAdapterFactory::AdapterMap();

//--------------------------------------------------------------------------------
void QtAdapterFactory::initDefaultAdapterMap()
{
    if (isDAMInitialized_)
        return;

#define CREATE_QTADAPTERFACTORY_ENTRY(qObjClassName, qtAdapterCreateFunc) \
    {                                                                   \
        QtAdapterInfo adapterInfo;                                      \
        adapterInfo.creatorFunc_ = qtAdapterCreateFunc;                 \
                                                                        \
        defaultAdapterMap_.insert(#qObjClassName, adapterInfo);         \
    }
#include QTADAPTERCLASSLIST_MACROFILE
#undef CREATE_QTADAPTERFACTORY_ENTRY

    isDAMInitialized_ = true;
}

//--------------------------------------------------------------------------------
QtAdapterFactory::QtAdapterFactory()
    : customAdapterMap_()
{
    initDefaultAdapterMap();
}

//--------------------------------------------------------------------------------
QtAdapter* QtAdapterFactory::getAdapter(const QObject * qObj, QtObject * adapterOwner)
{
    return getAdapter(qObj->metaObject()->className(), adapterOwner);
}

//--------------------------------------------------------------------------------
QtAdapter* QtAdapterFactory::getAdapter(const QString & qObjClassName, QtObject * adapterOwner)
{
    if (defaultAdapterMap_.contains(qObjClassName))
        return (defaultAdapterMap_.value(qObjClassName).creatorFunc_)(adapterOwner);
    if (customAdapterMap_.contains(qObjClassName))
        return (customAdapterMap_.value(qObjClassName).creatorFunc_)(adapterOwner);

    return NULL;
}

//--------------------------------------------------------------------------------
bool QtAdapterFactory::existsAdapter(const QString & qObjClassName)
{
    return (defaultAdapterMap_.contains(qObjClassName) ||
            customAdapterMap_.contains(qObjClassName));
}

//--------------------------------------------------------------------------------
void QtAdapterFactory::addCustomAdapter(const QString & qObjClassName, QtAdapter::CreatorFunc creatorFunc)
{
    QtAdapterInfo adapterInfo;
    adapterInfo.creatorFunc_ = creatorFunc;
    customAdapterMap_.insert(qObjClassName, adapterInfo);
}

