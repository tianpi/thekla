
#ifndef THEKLA_QDFORMMANGER_H
#define THEKLA_QDFORMMANGER_H

#include <QObject>
#include <QDesignerFormEditorInterface>
#include <QDesignerFormWindowManagerInterface>
#include <QMap>

class XmlTrashDocument;
class XmlElement;
class QtObject;
class QDFormWindowManager;
class Log;

//--------------------------------------------------------------------------------
/**
 * Maps QDesignerFormWindowInterface -> THEKLAFormWindowManager
 *
 * Observes the creation and destruction of FormWindow's
 *
 * Upon creation of a new FormWindow creates a new THEKLAFormWindowManager and passes a
 * reference of the FormWindow to the Collection
 *
 * Upon destruction of a FormWindow destructs the existing THEKLAFormWindowManager
 *
 * Implemented as singleton.
 *
 */
class QDFormManager : public QObject
{
    Q_OBJECT
private:
    static Log cLog;

    static QDFormManager * theInstance_;

public:
    /**
     * Initializes the singleton Instance.
     */
    static void init(QDesignerFormEditorInterface * formEditor);
    /**
     * Returns the singleton instance as soon as it was initialized. Before it returns
     * NULL.
     * @see init()
     */
    static QDFormManager* getInstance();

private:
    /**
     * The QDesigner FormEditor.
     */
    QDesignerFormEditorInterface * formEditor_;
    /**
     * The QDesigner FormManager (derived from FormEditor).
     */
    QDesignerFormWindowManagerInterface * formManager_;
    /**
     * Maps a FormWindow to its corresponding ObjectCollection.
     */
    QMap<const QDesignerFormWindowInterface *, QDFormWindowManager *> formWindowManagerMap_;
    /**
     * The currently active FormWindow.
     * @see activeFormWindowChanged()
     */
//    QDesignerFormWindowInterface * activeFormWindow_;
    /**
     * Objects contained in this container have been removed from a FormWindow. In case
     * that that Object was added in another FormWindow, it can be picked up from this
     * list by the corresponding THEKLAFormWindowManager.
     */
    XmlTrashDocument * trashDoc_;

private:
    /**
     * Retrieves the active QDFormWindow from the QDFormWindowManager (accessed via given
     * QDFormEditor) and calls add(QDesignerFormWindowInterface).
     *
     * Initializes the TrashDocument (a MasterDocument).
     */
    QDFormManager(QDesignerFormEditorInterface * formEditor);
    ~QDFormManager();

private:

    void init();

    void initFormWindowMap();


private slots:
    /**
     * Returns immediately if given FormWindow is already in the MAP (ERROR!). Otherwise
     * creates a ObjectCollection (which hopefully initializes itself correctly from an
     * existing file) and stores the pair [ FormWindow, ObjectCollection ] in the MAP.
     *
     * @see formWindowAdded()
     */
    void addFormWindow(QDesignerFormWindowInterface * formWin);
    /**
     * Returns immediately if given FormWindow is not contained in the MAP
     * (ERROR!). Otherwise destroys the ObjectCollection (without saving it as intended by
     * the user?) and removes the pair [ FormWindow, ObjectCollection ] from the MAP.
     *
     * @see formWindowRemoved()
     */
    void removeFormWindow(QDesignerFormWindowInterface * formWin);
    /**
     * @see activeFormWindow()
     */
//    void setActiveFormWindow ( QDesignerFormWindowInterface * formWindow );

public:
    /**
     * Returns the currently active FormWindow.
     * @see QDTaskMenuFactory::createExtension()
     */
    QDesignerFormWindowInterface * getActiveFormWindow() const
        { return formManager_->activeFormWindow(); };

    QDFormWindowManager * getActiveFormWindowManager() const;

    /**
     * Adds the given Object in the removed list. Returns true. If the given object is
     * already part of the list does nothing and returns false.
     */
    void addToTrash(QtObject * qtObj);
    /**
     * Queries the removed list for the given object. If there is such an entry, returns
     * the corresponding Object and returns the entry from the list. Otherwise return
     * NULL.
     */
    QtObject* getFromTrash(QObject * observable) const;

};

#endif // THEKLA_QDFORMMANGER_H
