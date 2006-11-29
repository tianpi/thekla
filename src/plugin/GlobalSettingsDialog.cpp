
#include "GlobalSettingsDialog.h"

#include <xml/UISettings.h>
#include <common/Log.h>

//--------------------------------------------------------------------------------
Log GlobalSettingsDialog::cLog(Log::getClassLog("GlobalSettingsDialog"));

//--------------------------------------------------------------------------------
GlobalSettingsDialog::GlobalSettingsDialog(UISettings * theklaSettings, QWidget * parent )
    : QDialog(parent),
      theklaSettings_(theklaSettings),
      isDirty_(false)
{
    Log log(Log::getMethodLog(cLog, "GlobalSettingsDialog()"));
    THEKLA_DEBUG(log, "** START.");

    //// Init Layout

    setupUi(this);

    //// Establish connections

    connect(applyButton, SIGNAL( clicked(bool) ),
            this, SLOT( applyButtonClicked() ));
    connect(resetButton, SIGNAL( clicked(bool) ),
            this, SLOT( resetButtonClicked() ));
    connect(okButton, SIGNAL( clicked(bool) ),
            this, SLOT( okButtonClicked() ));
    connect(cancelButton, SIGNAL( clicked(bool) ),
            this, SLOT( cancelButtonClicked() ));
    connect(useTheklaCBX, SIGNAL( toggled(bool) ),
            this, SLOT( useTheklaCBXClicked(bool) ));

    connect(useTheklaCBX, SIGNAL( clicked(bool) ),
            this, SLOT( setChanged() ));
    connect(generateQtAppCodeCHB, SIGNAL( clicked(bool) ),
            this, SLOT( setChanged() ));
    connect(serverHostLED, SIGNAL( textEdited(const QString &) ),
            this, SLOT( setChanged() ));
    connect(serverPortLED, SIGNAL( textEdited(const QString &) ),
            this, SLOT( setChanged() ));
    connect(serverCBPortLED, SIGNAL( textEdited(const QString &) ),
            this, SLOT( setChanged() ));
    connect(uiDataXPathPrefixLED, SIGNAL( textEdited(const QString &) ),
            this, SLOT( setChanged() ));


    //// Default contents

    QString title = tr("THEKLA Global Settings");
    setWindowTitle(title);

    resetChanges();
    useTheklaCBXClicked(useTheklaCBX->isChecked());

    THEKLA_DEBUG(log, "** END.");
}

//--------------------------------------------------------------------------------
GlobalSettingsDialog::~GlobalSettingsDialog()
{

}

//--------------------------------------------------------------------------------
void GlobalSettingsDialog::resetChanges()
{
    useTheklaCBX->setChecked(theklaSettings_->getXmlData().getConfigFlag(XmlElementData::USE_THEKLA));
    generateQtAppCodeCHB->setChecked(theklaSettings_->getXmlData().getConfigFlag(XmlElementData::GENERATE_QTAPP_CODE));
    serverHostLED->setText(theklaSettings_->getServerHost());
    serverPortLED->setText(QString::number(theklaSettings_->getServerPort()));
    serverCBPortLED->setText(QString::number(theklaSettings_->getServerCallbackPort()));
    uiDataXPathPrefixLED->setText(theklaSettings_->getUIDataXPathPrefix());
}

//--------------------------------------------------------------------------------
void GlobalSettingsDialog::applyChanges()
{
    theklaSettings_->getXmlData().setConfigFlag(XmlElementData::USE_THEKLA, useTheklaCBX->isChecked());
    theklaSettings_->getXmlData().setConfigFlag(XmlElementData::GENERATE_QTAPP_CODE, generateQtAppCodeCHB->isChecked());
    theklaSettings_->setServerHost(serverHostLED->text());
    theklaSettings_->setServerPort(serverPortLED->text().toInt());
    theklaSettings_->setServerCallbackPort(serverCBPortLED->text().toInt());
    theklaSettings_->setUIDataXPathPrefix(uiDataXPathPrefixLED->text());
}

//--------------------------------------------------------------------------------
void GlobalSettingsDialog::applyButtonClicked()
{
    isDirty_ = true;

    applyChanges();

    applyButton->setEnabled(false);
    resetButton->setEnabled(false);
}

//--------------------------------------------------------------------------------
void GlobalSettingsDialog::resetButtonClicked()
{
    resetChanges();

    applyButton->setEnabled(false);
    resetButton->setEnabled(false);
}

//--------------------------------------------------------------------------------
void GlobalSettingsDialog::okButtonClicked()
{
    applyChanges();
}

//--------------------------------------------------------------------------------
void GlobalSettingsDialog::cancelButtonClicked()
{
    // do nothing
}

//--------------------------------------------------------------------------------
void GlobalSettingsDialog::setChanged()
{
    applyButton->setEnabled(true);
    resetButton->setEnabled(true);
}

//--------------------------------------------------------------------------------
void GlobalSettingsDialog::useTheklaCBXClicked(bool checked)
{
    theklaGBX->setEnabled(checked);
    mwServerGBX->setEnabled(checked);
}

