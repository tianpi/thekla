/**
 * @file   Client.h
 * @author Christian Pirchheim
 *
 * @brief  Interface of class @c Client
 */

#ifndef THEKLA_CLIENT_H
#define THEKLA_CLIENT_H

#include <common/config-win32.h>

#include <common/Exception.h>
#include <common/DatabaseEventDispatcher.h>
#include <common/DatabaseConnManager.h>
#include <common/Observable.h>
#include <common/Observer.h>

#include <xml/XmlDatabaseDocument.h>
#include <xml/XmlConfigFileDocument.h>
#include <xml/UIDataRoot.h>
#include <xml/UISettings.h>
#include <xml/QtObject.h>
#include <xml/QtSignal.h>
#include <xml/QtProperty.h>
#include <xml/OivField.h>

#include <QString>
#include <QFile>


namespace THEKLA {

//--------------------------------------------------------------------------------
class THEKLA_EXPORT Client
{
public:

    enum AppType {
        QT_APP,
        NON_QT_APP
    };

private:

    bool isInitialized_;

    AppType appType_;

    Log clientLog_;

    DatabaseConn * dbConn_;

    DatabaseEventDispatcher * dbEventDispatcher_;

    XmlDatabaseDocument * dbDoc_;

private:

    Client();

    Client(const Client & copy);

    Client & operator=(const Client & lhs);

public:

    Client(AppType appType);

    virtual ~Client();

private:

    void initEnd();

    void initLog(const QString & appId);

    void initDbConn(const QString & serverHost, int serverPort, int serverCBPort);

public:

    void init(const QString & appId,
              const QString & serverHost, int serverPort, int serverCBPort,
              const QString & uiDataRootXPathPrefix = XmlDatabaseDocument::DEFAULT_UI_DATA_XPATH_PREFIX);

    void init(QFile & configFile, QObject * qObjectMainContainer, bool recoverFromDb = false);

    bool isInitialized() const
        { return isInitialized_; };

    void setReadInterval(unsigned int timeout)
        { dbEventDispatcher_->setProcessWatchdogRepliesTimeout(timeout); };

    void setWriteInterval(unsigned int timeout)
        { dbEventDispatcher_->setProcessUpdateRequestsTimeout(timeout); };

    void enableReading(bool enable)
        { dbEventDispatcher_->enableWatchdogRepliesProcessing(enable); };
    bool isReadingEnabled() const
        { return dbEventDispatcher_->isWatchdogRepliesProcessingEnabled(); };

    void enableWriting(bool enable)
        { dbEventDispatcher_->enableUpdateRequestsProcessing(enable); };
    bool isWritingEnabled() const
        { return dbEventDispatcher_->isUpdateRequestsProcessingEnabled(); };

    void connectRaiseEdgeOnDbChange(SoSFBool * field)
        { dbConn_->notifyOnDbChange(field); };

    void connectTouchOnDbChange(SoSFTrigger * field)
        { dbConn_->notifyOnDbChange(field); };

    void connectEmitOnDbChange(QObject * target, const char * slotSignature)
        { dbConn_->notifyOnDbChange(target, slotSignature); };

    XmlDatabaseDocument * getDatabaseDoc()
        { return dbDoc_; };


    //////// <InputClient>

    QtObject* insertQtObject(QObject * qObj, bool recoverFromDb = false);

    QtSignal* bindQtSignal(QtObject * qtObject, const QString & signature);

    QtProperty* bindQtProperty(QtObject * qtObject, const QString & propertyName);

    OivField* insertOivField(SoField * observable, const QString & fieldName, bool recoverFromDb = false);

    OivField* bindOivField(OivField * oivField);


    //////// </InputClient>


    //////// <OutputClient>

    //// SELECT

    QtObject * selectQtObject(const QString & qClassName, const QString & qObjName);

    QtSignal * selectQtSignal(QtObject * qtObject, const QString & qSignalSignature);

    QtProperty * selectQtProperty(QtObject * qtObject,
                                  const QString & qPropertyName, const QString & qPropertyType);

    OivField * selectOivField(const QString & oivClassName, const QString & oivObjName);

    //// CONNECT

    void connectQtSignalToQtSlot(QtSignal * qtSignal,
                                 QObject * qObj, const QString & qSlotSignature);

    void connectQtPropertyToQtProperty(QtProperty * qtProperty,
                                       QObject * qObj, const QString & qPropertyName);

    void connectQtSignalToOivFieldList(QtSignal * qtSignal,
                                       SoFieldList * fieldList);

    void connectQtPropertyToOivField(QtProperty * qtProperty,
                                     SoField * field);

    void connectOivFieldToQtProperty(OivField * oivField,
                                     QObject * qObj, const QString & qPropertyName);

    void connectOivFieldToOivField(OivField * oivField,
                                   SoField * field);

    //////// </OutputClient>

};

typedef Client InputClient;

typedef Client OutputClient;

} // namespace THEKLA

#endif // THEKLA_CLIENT_H
