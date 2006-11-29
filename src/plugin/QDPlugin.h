/**
 * @file   QDPlugin.h
 * @author Christian Pirchheim
 *
 * @brief  Interface of class @c QDPlugin
 */

#ifndef THEKLA_QDPLUGIN_H
#define THEKLA_QDPLUGIN_H

#include "QDWidget.h"

#include <common/Log.h>

#include <QtDesigner/QDesignerCustomWidgetInterface>
#include <QWidget>
#include <QIcon>

class QDFormManager;
class Log;
class QDWidget;

//--------------------------------------------------------------------------------
/**
 * Needed to access the lovely QDesignerFormEditorInterface object.
 *
 * Initializes the THEKLA TaskMenu Extension.
 *
 * Initializing the THEKLA plugin at startup of QDesigner.
 */
class QDPlugin: public QObject, public QDesignerCustomWidgetInterface
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)

private:
    static Log cLog;

private:

    bool isInitialized_;

    Log pluginLog_;

public:

    QDPlugin(QObject *parent = 0);
    virtual ~QDPlugin();

public:

    QString name() const
        { return "Thekla (dummy)"; };
    QString group() const
        { return "Thekla [DO NOT USE]"; };
    QString toolTip() const
        { return "Do NOT use!"; };
    QString whatsThis() const
        { return "Do NOT use!"; };
    QString includeFile() const
        { return ""; };
    /**
     * Returns something like a "DO NOT USE ME" Icon.
     */
    QIcon icon() const
        { return QIcon(); };
    bool isContainer() const
        { return false; };
    /**
     * Returns a quite stupid THEKLA widget that MUST NOT be used.
     */
    QWidget * createWidget(QWidget *parent)
        { return new QDWidget(parent); };
    bool isInitialized() const
        { return isInitialized_; };

    /**
     * Registers the TaskMenuFactory at the Qt-Designer ExtensionFactory.  Initializes the
     * THEKLAFormManager!!! Called exactly once: Sets the @e initialized bit to @e true after
     * first completion.
     *
     * @see THEKLAFormManager::init(QDesignerFormEditorInterface*),
     */
    void initialize(QDesignerFormEditorInterface *formEditor);

};

#endif // THEKLA_QDPLUGIN_H
