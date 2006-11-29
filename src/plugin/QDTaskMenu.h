
#ifndef THEKLA_QDTASKMENU_H
#define THEKLA_QDTASKMENU_H

#include <QObject>
#include <QAction>
#include <QtDesigner/QDesignerTaskMenuExtension>
#include <QtDesigner/QExtensionFactory>

class Log;
class QtAdapterFactory;

//--------------------------------------------------------------------------------
class QDTaskMenuFactory: public QExtensionFactory
{

    Q_OBJECT

private:
    static Log cLog;

private:
    QtAdapterFactory * adapterFactory_;

public:
    QDTaskMenuFactory(QExtensionManager * parent = 0);
    ~QDTaskMenuFactory();

protected:

    /**
     * Invoked before the context menu for @e object pops up. Returns NULL if none such is
     * desired otherwise return a TaskMenuExtension object.
     *
     * Returns a QDTaskMenu object.
     */
    QObject *createExtension(QObject *object, const QString &iid, QObject *parent) const;
};

//--------------------------------------------------------------------------------
/**
 * Provides means to add CONTEXT MENU options if a FormWindow Object is right-clicked.
 * The additional options are:
 *
 * @li "THEKLA Widget Settings"
 * @li "THEKLA Global Settings"
 */
class QDTaskMenu: public QObject, public QDesignerTaskMenuExtension
{
    Q_OBJECT
    Q_INTERFACES(QDesignerTaskMenuExtension);

private:
    static Log cLog;

private:
    /**
     * The Object in the FormWindow which has been right-clicked.
     */
    QObject * rightClickedObj_;
    /**
     * Provides signal which should be connected to a slot opening the THEKLA Settings
     * dialog for the actual QWidget (sorry for the inconsistence - but it IS a QWidget,
     * not a QObject)
     */
    QAction * showObjectSettingsDialog_;
    /**
     * Provides a signal which should be connected to a slot opening the THEKLA global
     * settings dialog.
     */
    QAction * showGlobalSettingsDialog_;

public:
    /**
     * Creates actions to be displayed as selections in a context menu for @e
     * rightClickedObj. Connect action signals with slots defined below.
     */
    QDTaskMenu(QObject * rightClickedObj, QObject * parent = NULL);
    ~QDTaskMenu();

public:
    /**
     * Return the QAction to be invoked if @<F2@> key was pressed. If none is desired,
     * returns NULL.
     */
    QAction* preferredEditAction() const;
    /**
     * Returns the list of actions to add to a context menu.
     */
    QList<QAction*> taskActions() const;

private slots:

    // Slot part of a connection defined in the constructor. Connected to one or more
    // actions.

    /**
     * Triggers the display of the THEKLA Object Settings dialog for the right-clicked
     * FormWindow object.
     *
     * @see THEKLAFormWindowManager::showObjectSettingsDialog(QObject * rightClickedObj);
     */
    void triggerObjectSettingsDialog();
    /**
     * Triggers the display of the THEKLA Global Settings dialog.
     *
     * @see THEKLAFormWindowManager::showGlobalSettingsDialog();
     */
    void triggerGlobalSettingsDialog();

};

#endif // THEKLA_QDTASKMENU_H
