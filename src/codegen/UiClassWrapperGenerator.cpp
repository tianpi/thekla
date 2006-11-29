
#include "UiClassWrapperGenerator.h"

#include <xml/XmlConfigFileDocument.h>
#include <xml/UIDataRoot.h>
#include <xml/UISettings.h>
#include <common/QDPluginUtils.h>


//--------------------------------------------------------------------------------
UiClassWrapperGenerator::UiClassWrapperGenerator(const XmlConfigFileDocument & fileDoc)
    : CxxCodeGenerator(),
      uiDataRoot_(fileDoc.getUIDataRoot()),
      uiSettings_(fileDoc.getUISettings()),
      theklaClientInstanceName_()
{
}

//--------------------------------------------------------------------------------
void UiClassWrapperGenerator::preProcess()
{
    className_ = "THEKLA_Ui_" + uiDataRoot_->getAppId();
    baseClassName_ = "Ui::" + uiDataRoot_->getAppId();

    QString uiHeaderFilePath = QDPluginUtils::getUiHeaderFilePath(uiSettings_->getUiFileName());
    localIncludeList_.push_back(QDPluginUtils::getFileName(uiHeaderFilePath));
    globalIncludeList_.push_back("client/Client.h");
    globalIncludeList_.push_back("QFile");

    theklaClientInstanceName_ = "theklaClient_";

}

//--------------------------------------------------------------------------------
void UiClassWrapperGenerator::codeClassDecl()
{
    code_ << "class " + className_ + " : public " + baseClassName_;
}

//--------------------------------------------------------------------------------
void UiClassWrapperGenerator::codeConstructor()
{
    code_ << className_ + "()";
    code_ << ": " + theklaClientInstanceName_ + "(THEKLA::Client::QT_APP)";
    code_ << "{}";
}

//--------------------------------------------------------------------------------
void UiClassWrapperGenerator::codeDestructor()
{
    code_ << "virtual ~" + className_ + "()";
    code_ << "{";
    code_ << "}";
}

//--------------------------------------------------------------------------------
void UiClassWrapperGenerator::codeSetupUiMethod()
{
    code_ << "void setupUi(" + uiSettings_->getMainContainerType() + " * " + uiDataRoot_->getAppId() + ")";
    code_ << "{";
    code_ << baseClassName_ + "::setupUi(" + uiDataRoot_->getAppId() + ");";
    code_ << "QFile configFile(\"" + QDPluginUtils::getConfigFilePath(uiSettings_->getUiFileName()).replace('\\', '/') + "\");";
    code_ << theklaClientInstanceName_ + ".init(configFile, " + uiDataRoot_->getAppId() + ");";
    code_ << "}";
}

//--------------------------------------------------------------------------------
void UiClassWrapperGenerator::generateCode()
{
    codeHeaderHead();
    codeLocalIncludes();
    codeGlobalIncludes();
    codeForwardDecls();
    codeClassDecl();
    code_ << "{";
    code_ << "protected:";
    code_ << "THEKLA::InputClient " + theklaClientInstanceName_ + ";";
    code_ << "public:";
    codeConstructor();
    codeDestructor();
    code_ << "public:";
    codeSetupUiMethod();
    code_ << "};";
    code_ << "";
    code_ << "namespace Ui {";
    code_ << "namespace THEKLA {";
    code_ << "class " + uiDataRoot_->getAppId() + " : public " + className_ + " {};";
    code_ << "}";
    code_ << "}";
    codeHeaderFoot();
    code_ << "";
}

