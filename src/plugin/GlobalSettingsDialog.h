
#ifndef THEKLA_GLOBALSETTINGSDIALOG_H
#define THEKLA_GLOBALSETTINGSDIALOG_H

#include "ui_GlobalSettingsDialog.h"

#include <QDialog>

class UISettings;
class Log;

//--------------------------------------------------------------------------------
class GlobalSettingsDialog : public QDialog, public Ui::GlobalSettingsDialog
{
    Q_OBJECT

private:
    static Log cLog;

private:

    UISettings * theklaSettings_;

    bool isDirty_;

private:
    GlobalSettingsDialog(const GlobalSettingsDialog & copy);

public:

    GlobalSettingsDialog(UISettings * theklaSettings_, QWidget * parent = NULL);
    virtual ~GlobalSettingsDialog();

private:

    void resetChanges();

    void applyChanges();


private slots:

    /**
     * Every Signal/Property Widget is connnected to this slot. Sets the dirty flag to
     * TRUE if currently false.
     *
     * At the time the dirty flag goes from FALSE -> TRUE, the "APPLY" and "RESET" buttons
     * must be enabled.
     */
    void setChanged();

    /**
     * Triggered by the "RESET" Button.
     *
     * Restores the current state of DOM structure within the SelectionWidgets. Iterates
     * through the SelectionWidgets calling method reset().
     *
     * Resets the dirty flag to FALSE.
     */
    void resetButtonClicked();
    /**
     * Triggered by the "APPLY" Button.
     *
     * Saves the current changes to the DOM structure. This is done by iterating through
     * all Signal- AND Property- SelectionWidgets (with the help of the lists) and calling
     * method save().
     *
     * Resets the dirty flag to FALSE.
     */
    void applyButtonClicked();
    /**
     * Triggered by the "OK" Button.
     *
     * Does "APPLY" the changes and shutdowns the dialog.
     */
    void okButtonClicked();
    /**
     * Triggered by the "CANCEL" Button.
     *
     * Shuts down the dialog.
     */
    void cancelButtonClicked();

    void useTheklaCBXClicked(bool checked);

public:

    bool isDirty() const { return isDirty_; };

};

#endif // THEKLA_GLOBALSETTINGSDIALOG_H
