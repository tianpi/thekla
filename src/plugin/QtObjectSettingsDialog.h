
#ifndef THEKLA_QTOBJECTSETTINGSDIALOG_H
#define THEKLA_QTOBJECTSETTINGSDIALOG_H

#if WIN32
#include <ui_QtObjectSettingsDialog.h>
#include <ui_QtObjectClassSelectionWidget.h>
#include <ui_QtSignalSettingsWidget.h>
#include <ui_QtPropertySettingsWidget.h>
#else
#include "ui_QtObjectSettingsDialog.h"
#include "ui_QtObjectClassSelectionWidget.h"
#include "ui_QtSignalSettingsWidget.h"
#include "ui_QtPropertySettingsWidget.h"
#endif

#include <QObject>
#include <QWidget>
#include <QList>
#include <QScrollArea>
#include <QSize>

class QtObject;
class QtSignal;
class QtProperty;
class Log;

//--------------------------------------------------------------------------------
class QtSignalSettingsWidget : public QWidget, public Ui::QtSignalSettingsWidget
{
    Q_OBJECT

private:
    QtSignal * qtSignal_;

public:
    QtSignalSettingsWidget(QtSignal * qtSignal, QWidget * parent = NULL);
    virtual ~QtSignalSettingsWidget()
        {};

public:
    void resetChanges();
    void applyChanges();

private slots:
    void setChanged();

signals:
    void widgetChanged();
};

//--------------------------------------------------------------------------------
class QtPropertySettingsWidget : public QWidget, public Ui::QtPropertySettingsWidget
{
    Q_OBJECT

private:
    QtProperty * qtProperty_;

public:
    QtPropertySettingsWidget(QtProperty * qtProperty, QWidget * parent = NULL);
    virtual ~QtPropertySettingsWidget()
        {};

public:
    void resetChanges();
    void applyChanges();

private slots:
    void setChanged();

signals:
    void widgetChanged();
};


//--------------------------------------------------------------------------------
class QtObjectClassSelectionWidget : public QWidget, public Ui::QtObjectClassSelectionWidget
{
    Q_OBJECT

private:
    const QMetaObject * qObjMeta_;

public:
    QtObjectClassSelectionWidget(const QMetaObject * qObjMeta, QWidget * parent = NULL);
    virtual ~QtObjectClassSelectionWidget()
        {};

public:
    void setActiveClass(const QString & className);

private slots:
    void setChanged();

signals:
    void activeClassChanged(const QString & className);
};


//--------------------------------------------------------------------------------
class SelectionScrollArea : public QScrollArea
{
    Q_OBJECT

public:
    SelectionScrollArea(QWidget * parent = NULL);
    virtual ~SelectionScrollArea()
        {};
};


//--------------------------------------------------------------------------------
class ScrollAreaContainerWidget : public QWidget
{
    Q_OBJECT

private:
    static Log cLog;

public:
    QVBoxLayout * layout;
    QSpacerItem * horizontalSpacer;

public:
    ScrollAreaContainerWidget(QWidget * parent = NULL);
    virtual ~ScrollAreaContainerWidget();

public:
    void pushBackWidget(QWidget * widget);

    void pushFrontWidget(QWidget * widget);

    QList<QWidget*> clearWidgets();

};


//--------------------------------------------------------------------------------
class QtObjectSettingsDialog : public QDialog, public Ui::QtObjectSettingsDialog
{
    Q_OBJECT

private:
    static Log cLog;

private:

    QtObject * qtObject_;

    QObject * qObject_;

    QList<QtSignalSettingsWidget*> signalWidgetList_;

    QList<QtPropertySettingsWidget*> propertyWidgetList_;

    bool isDirty_;

public:

    QtObjectClassSelectionWidget * classSelectWidget[2];

    SelectionScrollArea * scrollArea[2];

    ScrollAreaContainerWidget * scrollAreaContainerWidget[2];

private:
    QtObjectSettingsDialog(const QtObjectSettingsDialog & copy);

public:
    /**
     * Stores the given data internally.
     */
    QtObjectSettingsDialog(QtObject * qtObj, QWidget * parent = NULL);
    ~QtObjectSettingsDialog();

private:

    const QMetaObject* getMetaObject(const QString & className);


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
     * Another ObjectClassName has been choosen in the SignalTab. Clears the current
     * SelectionContainer widgets. Passes the new list of SignalWidgets to the
     * SelectionContainer.
     */
    void signalTabClassNameChanged(const QString & className);
    /**
     * The same as above for the Property SelectionContainer.
     */
    void propertyTabClassNameChanged(const QString & className);
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

public:
    /**
     * Retrieves the QObject from the QtObject. Throws an exception if it is NULL (thus
     * not available).
     *
     * Establishes the Dialog geometry: Passes the meta object to ClassSelection. Enables
     * the SIGNAL TAB per default. Establishes the signal/slot connections between the
     * Signal/Property Widgets, the ClassSelection Widget and the Dialog. Connects the
     * DialogButtons to the appropriate slots (saveButtonClicked() etc).
     *
     * Creates the Signal- and PropertySelection lists. The lists MUST EXACTLY MIRROR the
     * indices of QMetaObject::method(int) resp QMetaObject::property(int).
     *
     * Sets the default class in ClassSelection. Passes the appropriate Signal/Property
     * widgets to the SelectionContainers.
     *
     * Sets the dirty flag to FALSE.
     */
    void init();

    bool isDirty() const { return isDirty_; };

};

#endif // THEKLA_QTOBJECTSETTINGSDIALOG_H
