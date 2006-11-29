
#include "QDFormWindowManager.h"
#include "QDFormWindowUiFileObserver.h"
#include "QDFormManager.h"
#include "QtObjectSettingsDialog.h"
#include "GlobalSettingsDialog.h"

#include <xml/XmlConfigFileDocument.h>
#include <xml/QtObject.h>
#include <common/Exception.h>
#include <common/Log.h>
#include <xml/UIDataRoot.h>
#include <xml/UISettings.h>
#include <common/QDPluginUtils.h>

#include <codegen/UiClassWrapperGenerator.h>
#include <codegen/QtAppMainGenerator.h>
#include <codegen/QtAppProjectGenerator.h>


//--------------------------------------------------------------------------------
Log QDFormWindowManager::cLog(Log::getClassLog("QDFormWindowManager"));

//--------------------------------------------------------------------------------
QDFormWindowManager::QDFormWindowManager(QDFormManager * formManager,
                                         QDesignerFormWindowInterface * formWindow)
    : QObject(NULL),
      formManager_(formManager),
      formWindow_(formWindow),
      fileDoc_(new XmlConfigFileDocument),
      uiFileObserver_(new QDFormWindowUiFileObserver(this)),
      isDirty_(false),
      isInitialized_(false)
{
    Log log(Log::getMethodLog(cLog, "QDFormWindowManager()"));
    THEKLA_DEBUG(log, "** START: formManager = " << formManager << ", formWindow = " << formWindow);

    connect(formWindow_, SIGNAL(mainContainerChanged ( QWidget *  )),
            this, SLOT( init () ));

    THEKLA_DEBUG(log, "** END");
}

//--------------------------------------------------------------------------------
QDFormWindowManager::~QDFormWindowManager()
{
    Log log(Log::getMethodLog(cLog, "~QDFormWindowManager"));

    delete uiFileObserver_;
    delete fileDoc_;

    THEKLA_INFO(log, "** END.");
}

//--------------------------------------------------------------------------------
void QDFormWindowManager::init()
{
    Log log(Log::getMethodLog(cLog, "init()"));
    THEKLA_INFO(log, "** START.");

    if (isInitialized_)
        return;

    disconnect(SIGNAL(mainContainerChanged ( QWidget *  )),
               this, SLOT( init () ));

    //// init ConfigFile Document

    initXml();

    //// init UiFileObserver

    connect(formWindow_, SIGNAL(fileNameChanged ( const QString & )),
            uiFileObserver_, SLOT(setUiFileName ( const QString & )));
    connect(uiFileObserver_, SIGNAL(uiFileModified ( const QString & )),
            this, SLOT(saveToFile ( const QString & )));
    uiFileObserver_->start(formWindow_->fileName());

    //// init FormWindow connections

    connect(formWindow_, SIGNAL(widgetManaged ( QWidget * )),
            this, SLOT(addWidgetFromTrash ( QObject * )));
    connect(formWindow_, SIGNAL(widgetUnmanaged ( QWidget *  )),
            this, SLOT( removeWidget ( QObject *  )));

    isInitialized_ = true;

    THEKLA_INFO(log, "** END. isInitialized_ = " << isInitialized_);
}


//--------------------------------------------------------------------------------
void QDFormWindowManager::initXml()
{
    Log log(Log::getMethodLog(cLog, "initXml()"));
    THEKLA_DEBUG(log, "** START.");

    // init Document
    fileDoc_->init();

    //// load Xml from file

    THEKLA_DEBUG(log, "FormWindow.fileName() = " << formWindow_->fileName().toStdString());

    if (!formWindow_->fileName().isEmpty()) {

        QString configFileName = QDPluginUtils::getConfigFilePath(formWindow_->fileName());
        THEKLA_DEBUG(log, "configFileName = " << configFileName.toStdString());

        QFile loadFile(configFileName);
        if (loadFile.exists() && (loadFile.permissions() & QFile::ReadUser)) {

            THEKLA_DEBUG(log, "-- fileDoc.load()");

            try {
                fileDoc_->load(loadFile);
            } catch (Exception & ex) {
                THEKLA_WARN(log, ex.what());
            }
        }
    }

    THEKLA_DEBUG(log, "after load()");

    //// Merge document with main container QObject

    fileDoc_->mergeDocument(formWindow_->mainContainer());

    //// update settings

    updateSettings();

    THEKLA_DEBUG(log, "** END.");
}

//--------------------------------------------------------------------------------
void QDFormWindowManager::updateXml()
{
    Log log(Log::getMethodLog(cLog, "updateXml()"));
    THEKLA_DEBUG(log, "** START.");

    //// update elements (sync with Observables)

    fileDoc_->updateDocument();

    //// update settings

    updateSettings();

    THEKLA_DEBUG(log, "** END.");
}

//--------------------------------------------------------------------------------
void QDFormWindowManager::updateSettings()
{
    Log log(Log::getMethodLog(cLog, "updateSettings()"));
    THEKLA_DEBUG(log, "** START.");

    QWidget * mainContainer = formWindow_->mainContainer();
    THEKLA_DEBUG(log, "mainContainer = " << mainContainer);

    if (mainContainer == NULL) {
        THEKLA_DEBUG(log, "** END. mainContainer == NULL");
        return;
    }
    fileDoc_->getUIDataRoot()->setAppId(mainContainer->objectName());

    QString mainContType(mainContainer->metaObject()->className());
    fileDoc_->getUISettings()->setMainContainerType(mainContType.remove("Designer"));

    fileDoc_->getUISettings()->setUiFileName(formWindow_->fileName());

    THEKLA_DEBUG(log, "** END. objectName() = " << mainContainer->objectName().toStdString());
}

//--------------------------------------------------------------------------------
QtObject* QDFormWindowManager::addWidgetFromTrash(QObject * qObject)
{
    Log log(Log::getMethodLog(cLog, "addWidgetFromTrash()"));
    THEKLA_DEBUG(log, "** START. qObject = " << qObject);

    QtObject * qtObject = dynamic_cast<QtObject*>(fileDoc_->findElement(Observable(qObject)));

    // Element with given Observable already part of the Document
    if (qtObject != NULL) {
        THEKLA_DEBUG(log, "** END. findElement() != NULL");
        return qtObject;
    }

    //// lookup in trash bin

    qtObject = formManager_->getFromTrash(qObject);
    if (qtObject == NULL) {
        THEKLA_DEBUG(log, "** END. Nothing found in Trash.");
        return NULL;
    };

    THEKLA_DEBUG(log, "Found Element in Trash.");

    try {
        fileDoc_->addElement(qtObject);
    } catch (Exception & ex) {
        THEKLA_WARN(log, "addElement() Exception: " << ex.what());
        return NULL;
    }

    THEKLA_DEBUG(log, "** END. fileDoc.count() = " << fileDoc_->getUIDataRoot()->getChildCount());
    return qtObject;
}


//--------------------------------------------------------------------------------
QtObject* QDFormWindowManager::createWidget(QObject * qObject)
{
    Log log(Log::getMethodLog(cLog, "createWidget()"));
    THEKLA_DEBUG(log, "** START. qObject = " << qObject);

    QtObject * qtObject = dynamic_cast<QtObject*>(fileDoc_->findElement(Observable(qObject)));

    // Element with given Observable already part of the Document
    if (qtObject != NULL) {
        THEKLA_DEBUG(log, "** END. findElement() != NULL");
        return qtObject;
    }

    //// create new element for given Observable

    XmlSubDocument subDoc = fileDoc_->createSubDocument(Observable(qObject));
    qtObject = dynamic_cast<QtObject*>(subDoc.getRootElement());
    if (qtObject == NULL) {
        THEKLA_ERROR(log, "** END. subDoc.getRootElement() == NULL");
        return NULL;
    }

    THEKLA_DEBUG(log, "after createSubDocument()");

    try {
        fileDoc_->addElement(subDoc.getRootElement());
    } catch (Exception & ex) {
        THEKLA_WARN(log, "addElement() Exception: " << ex.what());
        delete qtObject;
        return NULL;
    }

    THEKLA_DEBUG(log, "** END. fileDoc.count() = " << fileDoc_->getUIDataRoot()->getChildCount());
    return qtObject;
}

//--------------------------------------------------------------------------------
QtObject* QDFormWindowManager::removeWidget(QObject * qObject)
{
    Log log(Log::getMethodLog(cLog, "removeWidget()"));
    THEKLA_DEBUG(log, "** START. qObject = " << qObject);

    // Element with given Observable NOT part of the Document
    XmlElement * qtObjElem = fileDoc_->findElement(Observable(qObject));
    if (qtObjElem == NULL) {
        THEKLA_WARN(log, "** END. Given Element NOT found. " << qObject);
        return NULL;
    }

    ////  update the element before we give it away to Trash

    fileDoc_->updateSubDocument(qtObjElem);


    //// remove element from Document

    qtObjElem = fileDoc_->removeElement(qtObjElem);


    //// put in trash bin of form manager

    QtObject * qtObject = dynamic_cast<QtObject*>(qtObjElem);
    formManager_->addToTrash(qtObject);

    THEKLA_DEBUG(log, "** END. fileDoc.count() = " << fileDoc_->getUIDataRoot()->getChildCount());
    return qtObject;
}

//--------------------------------------------------------------------------------
void QDFormWindowManager::saveToFile(const QString & uiFilePath)
{
    Log log(Log::getMethodLog(cLog, "saveToFile()"));
    THEKLA_DEBUG(log, "** START. uiFilePath = " << uiFilePath.toStdString());

    //// update DOM

    updateXml();

    UISettings * uiSettings = fileDoc_->getUISettings();

    //// Do NOT save if THEKLA was not used

    if (!uiSettings->getXmlData().getConfigFlag(XmlElementData::USE_THEKLA)) {
        THEKLA_DEBUG(log, "** END. USE_THEKLA == false.");
        return;
    }

    //// save Document to file

    QString configFilePath = QDPluginUtils::getConfigFilePath(uiFilePath);
    THEKLA_DEBUG(log, "configFilePath = " << configFilePath.toStdString());
    QFile saveFile(configFilePath);
    fileDoc_->save(saveFile);

    //// generate Qt application code

    if (uiSettings->getXmlData().getConfigFlag(XmlElementData::GENERATE_QTAPP_CODE)) {

        THEKLA_DEBUG(log, "GENERATE_QTAPP_CODE == true.");

        //// create and save Ui Wrapper header file

        UiClassWrapperGenerator uiClassWrapper(*fileDoc_);
        uiClassWrapper.exec();
        QString uiWrapperFilePath = QDPluginUtils::getUiWrapperFilePath(uiFilePath);
        THEKLA_DEBUG(log, "uiWrapperFilePath = " << uiWrapperFilePath.toStdString());
        CodeGenerator::saveToFile(uiWrapperFilePath, uiClassWrapper.getCode());

        //// create and save Qt Application main file

        QtAppMainGenerator qtAppMain(*fileDoc_);
        qtAppMain.exec();
        QString qtAppMainFilePath = QDPluginUtils::getQtAppMainFilePath(uiFilePath);
        THEKLA_DEBUG(log, "qtAppMainFilePath = " << qtAppMainFilePath.toStdString());
        CodeGenerator::saveToFile(qtAppMainFilePath, qtAppMain.getCode());

        //// create and save Qt Application project (.pro) file

        QtAppProjectGenerator qtAppProject(*fileDoc_);
        qtAppProject.exec();
        QString qtAppProjectFilePath = QDPluginUtils::getQtAppProjectFilePath(uiFilePath);
        THEKLA_DEBUG(log, "qtAppProjectFilePath = " << qtAppProjectFilePath.toStdString());
        CodeGenerator::saveToFile(qtAppProjectFilePath, qtAppProject.getCode());

    }

    // reset the dirty flag
    isDirty_ = false;

    THEKLA_INFO(log, "Written THEKLA settings successfully with uiFilePath (" << uiFilePath.toStdString() << ").");

    THEKLA_DEBUG(log, "** END. isDirty_ = " << isDirty_);
}

//--------------------------------------------------------------------------------
void QDFormWindowManager::showObjectSettingsDialog(QObject * rightClickedObj)
{
    Log log(Log::getMethodLog(cLog, "showObjectSettingsDialog()"));
    THEKLA_DEBUG(log, "** START. rightClickedObj = " << rightClickedObj);

    //// Retrieve a QtObject

    // create QtObject in Document if not exists
    QtObject * qtObject = createWidget(rightClickedObj);

    // check QtObject (pointer value NULL was clearly a bug)
    if (qtObject == NULL) {
        THEKLA_ERROR(log, "QtObject == NULL");
        return;
    }

    //// Init and execute the dialog (modal)

    QtObjectSettingsDialog dialog(qtObject);
    try {
        dialog.init();
    } catch (Exception & ex) {
        THEKLA_ERROR(log, "Init Dialog throws Exception: " << ex.what());
        return;
    }

    // start the modal (!) dialog in blocking
    dialog.exec();

    // TODO: set the FormWindow dirty flag (implementation in QtOSD incomplete!)
    formWindow_->setDirty(dialog.isDirty());
    // set the dirty flag ( (0,1), (1,0), (1,1) -> 1)
    isDirty_ |= dialog.isDirty();

    THEKLA_DEBUG(log, "** END. dialog.isDirty() = " << dialog.isDirty() << ", isDirty_ = " << isDirty_);

}

//--------------------------------------------------------------------------------
void QDFormWindowManager::showGlobalSettingsDialog()
{
    Log log(Log::getMethodLog(cLog, "showGlobalSettingsDialog()"));
    THEKLA_DEBUG(log, "** START.");

    //// Retrieve the settings element

    UISettings * theklaSettings = fileDoc_->getUISettings();
    if (theklaSettings == NULL) {
        THEKLA_ERROR(log, "UISettings == NULL");
        return;
    }

    //// Show Dialog

    GlobalSettingsDialog dialog(theklaSettings);

    // start the dialog blocking and modal (!)
    dialog.exec();

    // TODO: set the FormWindow dirty flag (implementation in QtOSD incomplete!)
    formWindow_->setDirty(dialog.isDirty());
    // set the dirty flag ( (0,1), (1,0), (1,1) -> 1)
    isDirty_ |= dialog.isDirty();

    THEKLA_DEBUG(log, "** END. dialog.isDirty() = " << dialog.isDirty() << ", isDirty_ = " << isDirty_);
}

//--------------------------------------------------------------------------------
bool QDFormWindowManager::isTHEKLAEnabled() const
{
    return fileDoc_->getUISettings()->getXmlData().getConfigFlag(XmlElementData::USE_THEKLA);
}

