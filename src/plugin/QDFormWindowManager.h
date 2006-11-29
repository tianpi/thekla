
#ifndef THEKLA_QDFORMWINDOWMANAGER_H
#define THEKLA_QDFORMWINDOWMANAGER_H

#include <QObject>
#include <QList>
#include <QDesignerFormWindowInterface>

class XmlConfigFileDocument;
class QDFormWindowUiFileObserver;
class Log;
class QDFormManager;
class QtObject;

//--------------------------------------------------------------------------------
/**
 * Manages the THEKLA Objects of one FormWindow.
 *
 * Initializes the Object list: if a saved .ui file exists, loads the THEKLA settings from
 * another related (and hopefully existing) file.
 *
 * OBSERVES the FormWindow:
 *
 * @li Removes an Object upon deletion from the FormWindow
 *
 * @li Adds an Object when added to the FormWindow (?) - but only if the QObject is
 * relevant for THEKLA: We have to find out if there exists a related OtQObject
 *
 * @li Saves the THEKLA settings (retrieved as XML from the QtQObject List): for that
 * purpose a Timer is installed which is started if the .ui filename is first set
 * (observed by the fileNameChanged() slot)
 *
 */
class QDFormWindowManager : public QObject // , public ClientBase
{
    Q_OBJECT
private:
    static Log cLog;

private:

    QDFormManager * formManager_;
    /**
     * The parent QDesigner FormWindow.
     */
    QDesignerFormWindowInterface * formWindow_;
    /**
     * The THEKLA XML config file document.
     */
    XmlConfigFileDocument * fileDoc_;

    QDFormWindowUiFileObserver * uiFileObserver_;

    bool isDirty_;

    bool isInitialized_;

private:
    QDFormWindowManager();
    QDFormWindowManager(QDFormWindowManager & copy);

public:
    /**
     * Creates a XmlFileDocument. Retrieves the ui-filename of the FormWindow. If there is
     * one, derives the THEKLA config file name ("form.ui" -> "form.thekla"). Tries to open
     * the file (exists, readable, writable) and - if ok - passes the file to the
     * XmlFileDocument which will set its contents appropriately.
     *
     * Retrieves the FormWindow main widget, retrieves its object name and sets the
     * Settings uiClassName appropriately. HINT: This is done by applying one of the
     * XmlDocument::findElement() methods, then down-casting (XmlElement -> MWSettings)
     * and finally calling the member method (setUiClassName()).
     *
     * Initializes the timer observing the uiFileName of the FormWindow and connects it to
     * slot saveFileTrigger(). Connects the FormWindow appropriate signal to slot
     * fileNameChanged().
     *
     * Connects the FormWindow to the widgetXXX() slots below: Observation of ADD/REMOVE
     * of QWidgets.
     *
     * @see QDesignerFormWindowInterface::fileName(), XmlFileDocument::XmlFileDocument(),
     * XmlFileDocument::load(QFile), QDesignerFormWindowInterface::mainWidget(),
     * XmlDocument::findFirstElement(QString), MWSettings::setUiClassName(QString)
     */
    QDFormWindowManager(QDFormManager * formManager,
                        QDesignerFormWindowInterface * formWindow);

    ~QDFormWindowManager();

private:

    void initXml();

    void updateXml();

    QtObject* createWidget(QObject * qObject);

    /**
     * QDFormWindow signal. Fetches its object name (equals UiClassName) and stores it
     * within the document DOM.
     */
    void updateSettings();

public:
    /**
     * Provides all necessary data to the dialog and shows it.
     * @see QDTaskMenu::triggerObjectSettingsDialog();
     *
     * Retrieves the QtObject from the Document (see
     * XmlElement::findElement(Observable)). If it does not exist, creates and adds it to
     * the Document. NOTE: The QtObject creates QtSignal and QtProperty children for ALL
     * signals and slots of the given QObject.
     *
     * Passes the QtObject to the QtObjectSettingsDialog.
     */
    void showObjectSettingsDialog(QObject * rightClickedObj);
    /**
     * Provides all necessary data to the dialog and shows it.
     * @see QDTaskMenu::triggerGlobalSettingsDialog();
     */
    void showGlobalSettingsDialog();

    bool isTHEKLAEnabled() const;

    bool isDirty() const
        { return isDirty_; };


private slots:

    void init();

    /**
     * Checks if given Object is already member of the MAP (ERROR if YES). Next, the
     * FormManager Removed list is queried. If no entry exists in the FormManager Removed
     * list, creates a new internal representation. Adds the pair [ QObject*, QtQObject* ]
     * to the MAP. Connects to the QObject signal destroyed().
     *
     * @see widgetManaged(), FormManager::getRemovedObject()
     */
    QtObject* addWidgetFromTrash(QObject * qObject);
    /**
     * Checks if given Object is member of the MAP (ERROR if NO). Adds the MAP entry to
     * the FormManager Removed list. Disconnects from the destroyed() signal. Removes the
     * pair [ QObject*, QtQObject* ] from the MAP.
     *
     * @see widgetManaged(), THEKLAFormManager::addRemovedObject()
     */
    QtObject* removeWidget(QObject * qObject);

public slots:

    /**
     * Saves the current THEKLA settings (global + ALL objects) to a file. Uses a QMutex
     * for mutual exclusion. The THEKLA settings filename is derived from the FormWindow
     * UI-filename.
     *
     * @see saveFileTrigger(), saveFileMutex_
     */
    void saveToFile(const QString & uiFilePath);


};

#endif // THEKLA_QDFORMWINDOWMANAGER_H
