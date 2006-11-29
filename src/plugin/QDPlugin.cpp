/**
 * @file   QDPlugin.cpp
 * @author Christian Pirchheim
 *
 * @brief  Implementation of class @c QDPlugin
 */

#include "QDPlugin.h"
#include "QDTaskMenu.h"
#include "QDFormManager.h"

#include <common/Exception.h>

#include <QDesignerFormEditorInterface>
#include <QExtensionManager>
#include <QtPlugin>

//--------------------------------------------------------------------------------
Log QDPlugin::cLog(Log::getClassLog("QDPlugin"));

//--------------------------------------------------------------------------------
QDPlugin::QDPlugin(QObject *parent )
    : QObject(parent),
      isInitialized_(false),
      pluginLog_(rootLog)
{
    // init Logging once
    if (!Log::isInitialized())
        Log::init();

    // init plugin logging
    pluginLog_ = (Log::getClientLog("Plugin"));
    Exception::setLog(pluginLog_);

//     try {
//         THROW_THEKLA_EXCEPTION("Test Exception.");
//     } catch (std::exception & ex) { }
}


//--------------------------------------------------------------------------------
QDPlugin::~QDPlugin()
{
}

//--------------------------------------------------------------------------------
void QDPlugin::initialize(QDesignerFormEditorInterface *formEditor)
{
    //// -> PLUGIN ENTRY POINT (method called by QD)

    try {

        Log log(Log::getMethodLog(cLog, "initialize()"));
        THEKLA_INFO(log, "** START. isInitialized_ = " << isInitialized_);

        // do nothing if already initialized
        if (isInitialized_) {
            return;
        }

        ////  register our factory

        QExtensionManager *manager = formEditor->extensionManager();
        manager->registerExtensions(new QDTaskMenuFactory(manager),
                                    Q_TYPEID(QDesignerTaskMenuExtension));

        //// Init our FormManager

        QDFormManager::init(formEditor);

        isInitialized_ = true;
        THEKLA_INFO(log, "** END. isInitialized_ = " << isInitialized_);

    } catch (std::exception & ex) {

        QString errMsg;
        QTextStream stream(&errMsg, QIODevice::WriteOnly);
        stream << "Exception within initialize()." << endl;
        stream << "Exception:" << endl;
        stream << ex.what() << endl;
        THEKLA_ERROR(pluginLog_, errMsg.toStdString());

        return;
    }

    THEKLA_INFO(pluginLog_, "Plugin initialized successfully.");
}

Q_EXPORT_PLUGIN2(taskmenuextension, QDPlugin);
