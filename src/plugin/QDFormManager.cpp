
#include "QDFormManager.h"
#include "QDFormWindowManager.h"
#include <xml/XmlTrashDocument.h>
#include <xml/XmlElement.h>
#include <common/Log.h>
#include <common/Exception.h>

//--------------------------------------------------------------------------------
Log QDFormManager::cLog(Log::getClassLog("QDFormManager"));
QDFormManager * QDFormManager::theInstance_ = NULL;

//--------------------------------------------------------------------------------
void QDFormManager::init(QDesignerFormEditorInterface * formEditor)
{
    Log log(Log::getMethodLog(cLog, "init()"));
    THEKLA_INFO(log, "** START: theInstance_ = " << theInstance_);

    if (theInstance_ != NULL)
        return;

    if (formEditor == NULL)
        THROW_THEKLA_EXCEPTION("Cannot initialize QDFormManager with QDesignerFormEditorInterface (NULL).");

    theInstance_ = new QDFormManager(formEditor);
    theInstance_->init();

    THEKLA_INFO(log, "** END: theInstance_ = " << theInstance_);
}

//--------------------------------------------------------------------------------
QDFormManager* QDFormManager::getInstance()
{
    return theInstance_;
}

//--------------------------------------------------------------------------------
QDFormManager::QDFormManager(QDesignerFormEditorInterface * formEditor)
    : formEditor_(formEditor),
      formManager_(formEditor_->formWindowManager()),
      formWindowManagerMap_(),
      trashDoc_(new XmlTrashDocument)
{
    Log log(Log::getMethodLog(cLog, "QDFormManager()"));
    THEKLA_DEBUG(log, "** START: formEditor_ = " << formEditor_);
    THEKLA_DEBUG(log, "** END.");
}

//--------------------------------------------------------------------------------
QDFormManager::~QDFormManager()
{
    formWindowManagerMap_.clear();
}

//--------------------------------------------------------------------------------
void QDFormManager::init()
{
    Log log(Log::getMethodLog(cLog, "init()"));
    THEKLA_DEBUG(log, "** START.");

    //// init FormWindowManagerMap

    initFormWindowMap();

    //// init FormManager connections

    connect(formManager_, SIGNAL(formWindowAdded ( QDesignerFormWindowInterface * )),
            this, SLOT(addFormWindow ( QDesignerFormWindowInterface * )));
    connect(formManager_, SIGNAL(formWindowRemoved ( QDesignerFormWindowInterface * )),
            this, SLOT(removeFormWindow ( QDesignerFormWindowInterface * )));
//     connect(formManager_, SIGNAL(activeFormWindowChanged ( QDesignerFormWindowInterface *)),
//             this, SLOT(setActiveFormWindow ( QDesignerFormWindowInterface *)));

    THEKLA_DEBUG(log, "** END.");

}


//--------------------------------------------------------------------------------
void QDFormManager::initFormWindowMap()
{
    Log log(Log::getMethodLog(cLog, "initFormWindowMap()"));
    THEKLA_DEBUG(log, "** START: formWindowCount() = " << formManager_->formWindowCount());

    //// create FormWindowManager for each existing FormWindow

    // NOTE: count must be zero if we initialize correcty (at the startup of QDesigner)
    for (int i = 0; i < formManager_->formWindowCount(); i++) {
        QDesignerFormWindowInterface * formWindow = formManager_->formWindow(i);
        addFormWindow(formWindow);
    }

    THEKLA_DEBUG(log, "** END: formWindowManagerMap_.size() = " << formWindowManagerMap_.size());
}


//--------------------------------------------------------------------------------
void QDFormManager::addFormWindow(QDesignerFormWindowInterface * formWindow)
{
    Log log(Log::getMethodLog(cLog, "addFormWindow()"));
    THEKLA_DEBUG(log, "** START: formWindowManagerMap_.size() = " << formWindowManagerMap_.size());

    //// Create FormWindowManager and add to map

    if (formWindowManagerMap_.contains(formWindow))
        return;

    QDFormWindowManager * formWindowManager = new QDFormWindowManager(this, formWindow);
    formWindowManagerMap_.insert(formWindow, formWindowManager);

    THEKLA_DEBUG(log, "** END: formWindowManagerMap_.size() = " << formWindowManagerMap_.size());
}

//--------------------------------------------------------------------------------
void QDFormManager::removeFormWindow(QDesignerFormWindowInterface * formWindow)
{
    Log log(Log::getMethodLog(cLog, "removeFormWindow()"));
    THEKLA_DEBUG(log, "** START: formWindowManagerMap_.size() = " << formWindowManagerMap_.size());

    //// Remove FormWindow from Map

    if (!formWindowManagerMap_.contains(formWindow))
        return;

    // get registered window(s) (must be exactly ONE)
    QDFormWindowManager * formWindowManager = formWindowManagerMap_.value(formWindow);

    // remove from map
    if (formWindowManagerMap_.remove(formWindow) != 1) {
        THEKLA_WARN(log, "formWindowManagerMap_ remove != 1");
    }

    //// delete FormManger

    THEKLA_DEBUG(log, "formWindowManager->isDirty() = " << formWindowManager->isDirty());
    THEKLA_DEBUG(log, "formWindow->isDirty() = " << formWindow->isDirty());

    // NOTE: Cannot be applied. Why? When you have a "dirty" form and try to close it
    // you're asked: "You wanna save?". And either you click "Yes" or "No" the
    // formWindow->isDirty() returns "false". So I cannot derive the user's will here ..
//     if (formWindowManager->isDirty() && !formWindow->isDirty()) {
//         THEKLA_DEBUG(log, "Called QDFormWindowManager::saveToFile().");
//         formWindowManager->saveToFile(formWindow->fileName());
//     }
    delete formWindowManager;

    THEKLA_DEBUG(log, "** END: formWindowManagerMap_.size() = " << formWindowManagerMap_.size());
}

//--------------------------------------------------------------------------------
void QDFormManager::addToTrash(QtObject * qtObj)
{
    Log log(Log::getMethodLog(cLog, "addToTrash()"));
    THEKLA_DEBUG(log, "** START: trashDoc_->getRootElement()->getChildCount() = "
                << trashDoc_->getRootElement()->getChildCount());

    trashDoc_->push(qtObj);

    THEKLA_DEBUG(log, "** END: trashDoc_->getRootElement()->getChildCount() = "
                << trashDoc_->getRootElement()->getChildCount());
}

//--------------------------------------------------------------------------------
QtObject* QDFormManager::getFromTrash(QObject * qObject) const
{
    Log log(Log::getMethodLog(cLog, "getFromTrash()"));
    THEKLA_DEBUG(log, "** START: trashDoc_->getRootElement()->getChildCount() = "
                << trashDoc_->getRootElement()->getChildCount());

    QtObject * ret = trashDoc_->get(qObject);

    THEKLA_DEBUG(log, "trashDoc_->getRootElement()->getChildCount() = "
                << trashDoc_->getRootElement()->getChildCount());
    THEKLA_DEBUG(log, "** END. ret = " << ret);
    return ret;
}

//--------------------------------------------------------------------------------
QDFormWindowManager * QDFormManager::getActiveFormWindowManager() const
{
    if (!formWindowManagerMap_.contains(formManager_->activeFormWindow()))
        return NULL;

    return formWindowManagerMap_.value(formManager_->activeFormWindow());
}

