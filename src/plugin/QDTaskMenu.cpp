
#include "QDTaskMenu.h"
#include "QDFormManager.h"
#include "QDFormWindowManager.h"

#include <adapter/QtAdapterFactory.h>
#include <common/Log.h>


//================================================================================

//--------------------------------------------------------------------------------
Log QDTaskMenuFactory::cLog(Log::getClassLog("QDTaskMenuFactory"));

//--------------------------------------------------------------------------------
QDTaskMenuFactory::QDTaskMenuFactory(QExtensionManager * parent )
    : QExtensionFactory(parent),
      adapterFactory_(new QtAdapterFactory)
{
}

//--------------------------------------------------------------------------------
QDTaskMenuFactory::~QDTaskMenuFactory()
{
    if (adapterFactory_ != NULL) delete adapterFactory_;
}

//--------------------------------------------------------------------------------
QObject * QDTaskMenuFactory::createExtension(QObject *object, const QString &iid, QObject *parent) const
{
    Log log(Log::getMethodLog(cLog, "createExtension()"));
    THEKLA_DEBUG(log, "** START. object = " << object
                << ", iid = " << iid.toStdString() << ", parent = " << parent);

    //// check parameters

    if (iid != Q_TYPEID(QDesignerTaskMenuExtension)) {
        THEKLA_DEBUG(log, "** END. iid != QDesignerTaskMenuExtension");
        return NULL;
    }

    if (object == NULL) {
        THEKLA_DEBUG(log, "** END. object == NULL.");
        return NULL;
    }

    //// query QtAdapterFactory

    QString qObjClassName(object->metaObject()->className());

    // example: rename QDesignerLabel -> QLabel
    qObjClassName.remove("Designer");
    THEKLA_DEBUG(log, "Exists Adapter for QObject with className = " << qObjClassName.toStdString());

    if (!adapterFactory_->existsAdapter(qObjClassName)) {
        THEKLA_DEBUG(log, "** END. QtAdapterFactory().existsAdapter() == false.");
        return NULL;
    }

    THEKLA_DEBUG(log, "** END. Ok.");
    return new QDTaskMenu(object, parent);
}

//================================================================================

//--------------------------------------------------------------------------------
Log QDTaskMenu::cLog(Log::getClassLog("QDTaskMenu"));

//--------------------------------------------------------------------------------
QDTaskMenu::QDTaskMenu(QObject * rightClickedObj, QObject * parent)
    : QObject(parent),
      rightClickedObj_(rightClickedObj),
      showObjectSettingsDialog_(NULL),
      showGlobalSettingsDialog_(NULL)
{
    Log log(Log::getMethodLog(cLog, "QDTaskMenu()"));
    THEKLA_DEBUG(log, "** START. rightClickedObj = " << rightClickedObj
                << ", parent = " << parent);

    //// init and connect Actions

    showObjectSettingsDialog_ = new QAction(tr("THEKLA Object Settings..."), this);
    connect(showObjectSettingsDialog_, SIGNAL( triggered() ),
            this, SLOT( triggerObjectSettingsDialog() ));

    showGlobalSettingsDialog_ = new QAction(tr("THEKLA Global Settings..."), this);
    connect(showGlobalSettingsDialog_, SIGNAL( triggered() ),
            this, SLOT( triggerGlobalSettingsDialog() ));

    THEKLA_DEBUG(log, "** END.");
}

//--------------------------------------------------------------------------------
QDTaskMenu::~QDTaskMenu()
{
}

//--------------------------------------------------------------------------------
QAction* QDTaskMenu::preferredEditAction() const
{
    if (QDFormManager::getInstance()->getActiveFormWindowManager()->isTHEKLAEnabled())
        return showObjectSettingsDialog_;

    return NULL;
}

//--------------------------------------------------------------------------------
QList<QAction*> QDTaskMenu::taskActions() const
{
    QList<QAction*> taskMenuActionList;

    if (QDFormManager::getInstance()->getActiveFormWindowManager()->isTHEKLAEnabled()) {
        taskMenuActionList.push_back(showObjectSettingsDialog_);
    }

    taskMenuActionList.push_back(showGlobalSettingsDialog_);

    return taskMenuActionList;
}

//--------------------------------------------------------------------------------
void QDTaskMenu::triggerObjectSettingsDialog()
{
    QDFormManager * formManager = QDFormManager::getInstance();
    QDFormWindowManager * formWindowManager = formManager->getActiveFormWindowManager();

    formWindowManager->showObjectSettingsDialog(rightClickedObj_);
}

//--------------------------------------------------------------------------------
void QDTaskMenu::triggerGlobalSettingsDialog()
{
    QDFormManager * formManager = QDFormManager::getInstance();
    QDFormWindowManager * formWindowManager = formManager->getActiveFormWindowManager();

    formWindowManager->showGlobalSettingsDialog();
}

