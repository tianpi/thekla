
#include "QtObjectSettingsDialog.h"

#include <xml/QtObject.h>
#include <xml/QtSignal.h>
#include <xml/QtProperty.h>
#include <common/Observable.h>
#include <common/Log.h>
#include <common/Exception.h>

//================================================================================

//--------------------------------------------------------------------------------
QtSignalSettingsWidget::QtSignalSettingsWidget(QtSignal * qtSignal, QWidget * parent )
    : QWidget(parent),
      qtSignal_(qtSignal)
{
    setupUi(this);

    signatureLabel->setText(qtSignal_->getSignature());
    resetChanges();

    connect(bindCB, SIGNAL(clicked(bool)), this, SLOT(setChanged()));
    connect(useAsTriggerCB, SIGNAL(clicked(bool)), this, SLOT(setChanged()));
}

//--------------------------------------------------------------------------------
void QtSignalSettingsWidget::resetChanges()
{
    bindCB->setChecked(qtSignal_->getXmlData().getConfigFlag(XmlElementData::BIND));
    useAsTriggerCB->setChecked(qtSignal_->getXmlData().getConfigFlag(XmlElementData::USE_AS_TRIGGER));
}

//--------------------------------------------------------------------------------
void QtSignalSettingsWidget::applyChanges()
{
    qtSignal_->getXmlData().setConfigFlag(XmlElementData::BIND, bindCB->isChecked());
    qtSignal_->getXmlData().setConfigFlag(XmlElementData::USE_AS_TRIGGER, useAsTriggerCB->isChecked());
}

//--------------------------------------------------------------------------------
void QtSignalSettingsWidget::setChanged()
{
    emit widgetChanged();
}

//================================================================================

//--------------------------------------------------------------------------------
QtPropertySettingsWidget::QtPropertySettingsWidget(QtProperty * qtProperty, QWidget * parent )
    : QWidget(parent),
      qtProperty_(qtProperty)
{
    setupUi(this);

    propertyLabel->setText(qtProperty_->getName() + " [" + qtProperty_->getType() + "]");
    resetChanges();

    connect(updateCB, SIGNAL(clicked(bool)), this, SLOT(setChanged()));
}

//--------------------------------------------------------------------------------
void QtPropertySettingsWidget::resetChanges()
{
    updateCB->setChecked(qtProperty_->getXmlData().getConfigFlag(XmlElementData::UPDATE));
}

//--------------------------------------------------------------------------------
void QtPropertySettingsWidget::applyChanges()
{
    qtProperty_->getXmlData().setConfigFlag(XmlElementData::UPDATE, updateCB->isChecked());
}

//--------------------------------------------------------------------------------
void QtPropertySettingsWidget::setChanged()
{
    emit widgetChanged();
}

//================================================================================

//--------------------------------------------------------------------------------
QtObjectClassSelectionWidget::QtObjectClassSelectionWidget(const QMetaObject * qObjMeta, QWidget * parent )
    : QWidget(parent),
      qObjMeta_(qObjMeta)
{
    setupUi(this);

    //// init CB

    for (const QMetaObject * meta = qObjMeta_; meta != NULL; meta = meta->superClass()) {
        qtObjectClassCB->addItem(meta->className());
    }
    setActiveClass(qObjMeta_->className());

    //// connect

    connect(qtObjectClassCB, SIGNAL( activated(const QString &) ),
            this, SLOT( setChanged() ));
}

//--------------------------------------------------------------------------------
void QtObjectClassSelectionWidget::setActiveClass(const QString & className)
{
    int index = qtObjectClassCB->findText(className);
    qtObjectClassCB->setCurrentIndex((index == -1) ? 0 : index);
}

//--------------------------------------------------------------------------------
void QtObjectClassSelectionWidget::setChanged()
{
    emit activeClassChanged(qtObjectClassCB->currentText());
}


//================================================================================

//--------------------------------------------------------------------------------
SelectionScrollArea::SelectionScrollArea(QWidget * parent )
    : QScrollArea(parent)
{
    setLineWidth(0);
    setWidgetResizable(true);
}

//================================================================================

Log ScrollAreaContainerWidget::cLog(Log::getClassLog("ScrollAreaContainerWidget"));

//--------------------------------------------------------------------------------
ScrollAreaContainerWidget::ScrollAreaContainerWidget(QWidget * parent)
    : QWidget(parent)
{
    resize(QSize(1, 1).expandedTo(this->minimumSizeHint()));
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);

    layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    layout->setMargin(0);

    // the spacer ALWAYS stays at the bottom of the layout
    horizontalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    // NOTE: The ownership of item is transferred to the layout, and it's the layout's
    // responsibility to delete it.
    layout->addItem(horizontalSpacer);
}

//--------------------------------------------------------------------------------
ScrollAreaContainerWidget::~ScrollAreaContainerWidget()
{
}

//--------------------------------------------------------------------------------
void ScrollAreaContainerWidget::pushBackWidget(QWidget * widget)
{
    // NOTE: QLayout::addChildWidget: QtPropertySettingsWidget "QtPropertySettingsWidget"
    // in wrong parent; moved to correct parent
    widget->setParent(this);

    // insert widget at the bottom but before the (final) horizontal spacer
    int index = layout->count() - 1 - 1;
    layout->insertWidget((index < 0) ? 0 : index, widget);
}

//--------------------------------------------------------------------------------
void ScrollAreaContainerWidget::pushFrontWidget(QWidget * widget)
{
    // NOTE: QLayout::addChildWidget: QtPropertySettingsWidget "QtPropertySettingsWidget"
    // in wrong parent; moved to correct parent
    widget->setParent(this);

    // insert widget at the head
    layout->insertWidget(0, widget);
}

//--------------------------------------------------------------------------------
QList<QWidget*> ScrollAreaContainerWidget::clearWidgets()
{
    Log log(Log::getMethodLog(cLog, "clearWidgets()"));
    THEKLA_DEBUG(log, "** START.");

    QList<QWidget*> widgetList;

    // remove all widgets except (bottom) the horizonal spacer
    int removeCount = layout->count() - 1;
    for (int i = 0; i < removeCount; i++) {
        QLayoutItem * item = layout->takeAt(0);
        if (item == NULL) {
            THEKLA_WARN(log, "QLayoutItem == NULL.");
            continue;
        }

        // MUST NOT DELETE !!
        QWidget * widget = item->widget();
        if (widget != NULL) {
            widget->setParent(NULL);
            widgetList.push_back(widget);
        }
    }

    THEKLA_DEBUG(log, "** END. widgetList.size() = " << widgetList.size());
    return widgetList;
}


//================================================================================

Log QtObjectSettingsDialog::cLog(Log::getClassLog("QtObjectSettingsDialog"));

//--------------------------------------------------------------------------------
QtObjectSettingsDialog::QtObjectSettingsDialog(QtObject * qtObj, QWidget * parent)
    : QDialog(parent),
      qtObject_(qtObj),
      qObject_(NULL),
      signalWidgetList_(),
      propertyWidgetList_(),
      isDirty_(false)
{
    Log log(Log::getMethodLog(cLog, "QtObjectSettingsDialog()"));
    THEKLA_DEBUG(log, "** START.");

    setupUi(this);

    //// Connections

    connect(applyButton, SIGNAL( clicked(bool) ),
            this, SLOT( applyButtonClicked() ));
    connect(resetButton, SIGNAL( clicked(bool) ),
            this, SLOT( resetButtonClicked() ));
    connect(okButton, SIGNAL( clicked(bool) ),
            this, SLOT( okButtonClicked() ));
    connect(cancelButton, SIGNAL( clicked(bool) ),
            this, SLOT( cancelButtonClicked() ));


    THEKLA_DEBUG(log, "** END.");
}

//--------------------------------------------------------------------------------
QtObjectSettingsDialog::~QtObjectSettingsDialog()
{
    Log log(Log::getMethodLog(cLog, "~QtObjectSettingsDialog()"));
    THEKLA_DEBUG(log, "** START.");

    for (int i = 0; i < signalWidgetList_.size(); i++) {
        if (signalWidgetList_[i]->parent() == NULL) {
            delete signalWidgetList_[i];
        }
    }
    signalWidgetList_.clear();

    for (int i = 0; i < propertyWidgetList_.size(); i++) {
        if (propertyWidgetList_[i]->parent() == NULL) {
            delete propertyWidgetList_[i];
        }
    }
    propertyWidgetList_.clear();

    THEKLA_DEBUG(log, "** END.");
}

//--------------------------------------------------------------------------------
void QtObjectSettingsDialog::init()
{
    Log log(Log::getMethodLog(cLog, "init()"));
    THEKLA_DEBUG(log, "** START.");

    //// Retrieve Observable of given QtOBject

    const Observable * qtObjObservable = qtObject_->getObservable();
    if (qtObjObservable == NULL) {
        THROW_THEKLA_EXCEPTION("Given QtObject ("
                              "(className = " << qtObject_->metaObject()->className()
                              << ", objectName() = " << qtObject_->objectName() << ") "
                              << "does not have an Observable.");
    }
    qtObjObservable->getObservable(qObject_);
    if (qObject_ == NULL) {
        THROW_THEKLA_EXCEPTION("Given QtObject "
                              << "(className = " << qtObject_->metaObject()->className()
                              << ", objectName() = " << qtObject_->objectName() << ") "
                              << "does have an INVALID Observable. Expected: QObject. Found: NULL pointer value).");
    }

    //// Init Dialog

    QString title = tr("THEKLA Object Settings:");
    title += " " + qObject_->objectName();
    title += " [" + QString(qObject_->metaObject()->className()) + "]";
    setWindowTitle(title);

    //// Init Signal, Property Map

    QList<QtSignal*> qtSignalList = qtObject_->getSignals();
    for (int i = 0; i < qtSignalList.size(); i++) {
        QtSignalSettingsWidget * signalWidget = new QtSignalSettingsWidget(qtSignalList[i], NULL);
        connect(signalWidget, SIGNAL( widgetChanged() ), this, SLOT( setChanged() ));
        signalWidgetList_.push_back(signalWidget);
    }

    QList<QtProperty*> qtPropertyList = qtObject_->getProperties();
    for (int i = 0; i < qtPropertyList.size(); i++) {
        QtPropertySettingsWidget * propertyWidget = new QtPropertySettingsWidget(qtPropertyList[i], NULL);
        connect(propertyWidget, SIGNAL( widgetChanged() ), this, SLOT( setChanged() ));
        propertyWidgetList_.push_back(propertyWidget);
    }

    //// Layout UI

    for (int i = 0; i < 2; i++) {

        // set parent Tab
        QWidget * parentTab = NULL;
        switch (i) {
        case 0: parentTab = signalTab; break;
        default: parentTab = propertyTab; break;
        }

        // QObject class selection widget
        classSelectWidget[i] = new QtObjectClassSelectionWidget(qObject_->metaObject(), parentTab);
        parentTab->layout()->addWidget(classSelectWidget[i]);

        // ScrollArea with container widget
        scrollArea[i] = new SelectionScrollArea(parentTab);
        scrollAreaContainerWidget[i] = new ScrollAreaContainerWidget(parentTab);
        scrollArea[i]->setWidget(scrollAreaContainerWidget[i]);
        parentTab->layout()->addWidget(scrollArea[i]);

    }

    //// Connections cont.

    connect(classSelectWidget[0], SIGNAL( activeClassChanged( const QString & )),
            this, SLOT( signalTabClassNameChanged(const QString &) ));
    connect(classSelectWidget[1], SIGNAL( activeClassChanged( const QString & )),
            this, SLOT( propertyTabClassNameChanged(const QString &) ));


    //// Init UI Contents

    signalTabClassNameChanged(qObject_->metaObject()->className());
    propertyTabClassNameChanged(qObject_->metaObject()->className());

    THEKLA_DEBUG(log, "** END.");
}

//--------------------------------------------------------------------------------
const QMetaObject * QtObjectSettingsDialog::getMetaObject(const QString & className)
{
    Log log(Log::getMethodLog(cLog, "getMetaObject()"));
    THEKLA_DEBUG(log, "** START. className = " << className.toStdString());

    for (const QMetaObject * meta = qObject_->metaObject();
         meta != NULL;
         meta = meta->superClass())
    {
        if (meta->className() == className) {
            THEKLA_DEBUG(log, "** END. meta.className() = " << meta->className());
            return meta;
        }
    }

    THEKLA_WARN(log, "** END. meta == NULL");
    return NULL;
}

//--------------------------------------------------------------------------------
void QtObjectSettingsDialog::signalTabClassNameChanged(const QString & className)
{
    Log log(Log::getMethodLog(cLog, "signalTabClassNameChanged()"));
    THEKLA_DEBUG(log, "** START.");

    const QMetaObject * qMetaObj = getMetaObject(className);
    if (qMetaObj == NULL)
        return;

    QList<QWidget*> widgetList = scrollAreaContainerWidget[0]->clearWidgets();
    for (int i = 0; i < widgetList.size(); i++) {
        widgetList[i]->setParent(NULL);
    }
    widgetList.clear();

    QList<QtSignal*> qtSignalList = qtObject_->getSignals();

    for (int i = qMetaObj->methodOffset(); i < qMetaObj->methodCount(); i++) {

        QMetaMethod qSignal = qMetaObj->method(i);
        if (qSignal.methodType() != QMetaMethod::Signal)
            continue;

        for (int j = 0; j < qtSignalList.size(); j++) {
            if (qtSignalList[j]->getSignature() == qSignal.signature()) {
                scrollAreaContainerWidget[0]->pushBackWidget(signalWidgetList_[j]);
            }
        }
    }

    THEKLA_DEBUG(log, "** END.");
}

//--------------------------------------------------------------------------------
void QtObjectSettingsDialog::propertyTabClassNameChanged(const QString & className)
{
    Log log(Log::getMethodLog(cLog, "propertyTabClassNameChanged()"));
    THEKLA_DEBUG(log, "** START.");

    const QMetaObject * qMetaObj = getMetaObject(className);
    if (qMetaObj == NULL)
        return;

    QList<QWidget*> widgetList = scrollAreaContainerWidget[1]->clearWidgets();
    for (int i = 0; i < widgetList.size(); i++) {
        widgetList[i]->setParent(NULL);
    }
    widgetList.clear();

    QList<QtProperty*> qtPropertyList = qtObject_->getProperties();

    for (int i = qMetaObj->propertyOffset(); i < qMetaObj->propertyCount(); i++) {

        QMetaProperty qProperty = qMetaObj->property(i);

        for (int j = 0; j < qtPropertyList.size(); j++) {
            if (qtPropertyList[j]->getName() == qProperty.name() &&
                qtPropertyList[j]->getType() == qProperty.typeName()) {
                scrollAreaContainerWidget[1]->pushBackWidget(propertyWidgetList_[j]);
            }
        }
    }

    THEKLA_DEBUG(log, "** END.");
}

//--------------------------------------------------------------------------------
void QtObjectSettingsDialog::setChanged()
{
    // set button states
    applyButton->setEnabled(true);
    resetButton->setEnabled(true);
}

//--------------------------------------------------------------------------------
void QtObjectSettingsDialog::resetButtonClicked()
{
    //// Save signal, property settings

    for (int i = 0; i < signalWidgetList_.size(); i++) {
        signalWidgetList_[i]->resetChanges();
    }
    for (int i = 0; i < propertyWidgetList_.size(); i++) {
        propertyWidgetList_[i]->resetChanges();
    }

    // set button states
    applyButton->setEnabled(false);
    resetButton->setEnabled(false);

}


//--------------------------------------------------------------------------------
void QtObjectSettingsDialog::applyButtonClicked()
{
    // set the dirty flag
    isDirty_ = true; // applyButton->isEnabled();

    //// Save signal, property settings

    for (int i = 0; i < signalWidgetList_.size(); i++) {
        signalWidgetList_[i]->applyChanges();
    }
    for (int i = 0; i < propertyWidgetList_.size(); i++) {
        propertyWidgetList_[i]->applyChanges();
    }

    // set button states
    applyButton->setEnabled(false);
    resetButton->setEnabled(false);
}

//--------------------------------------------------------------------------------
void QtObjectSettingsDialog::okButtonClicked()
{
    // save signal + property settings
    applyButtonClicked();
}

//--------------------------------------------------------------------------------
void QtObjectSettingsDialog::cancelButtonClicked()
{
    // do nothing
}


