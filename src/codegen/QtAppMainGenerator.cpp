
#include "QtAppMainGenerator.h"

#include <xml/XmlConfigFileDocument.h>
#include <xml/UIDataRoot.h>
#include <xml/UISettings.h>
#include <common/QDPluginUtils.h>

//--------------------------------------------------------------------------------
QtAppMainGenerator::QtAppMainGenerator(const XmlConfigFileDocument & fileDoc)
    : UiClassWrapperGenerator(fileDoc)
{
}

//--------------------------------------------------------------------------------
void QtAppMainGenerator::preProcess()
{
    QString uiWrapperPath = QDPluginUtils::getUiWrapperFilePath(uiSettings_->getUiFileName());
    localIncludeList_.push_back(QDPluginUtils::getFileName(uiWrapperPath));
    globalIncludeList_.push_back("QApplication");
    globalIncludeList_.push_back("stdexcept");
}

//--------------------------------------------------------------------------------
void QtAppMainGenerator::generateCode()
{
    codeLocalIncludes();
    codeGlobalIncludes();
    codeForwardDecls();
    code_ << "int main(int argc, char* argv[])";
    code_ << "{";
    code_ << "QApplication a(argc, argv);";
    code_ << uiSettings_->getMainContainerType() + " container;";
    code_ << "Ui::THEKLA::" + uiDataRoot_->getAppId() + " ui;";
    code_ << "try {";
    code_ << "ui.setupUi(&container);";
    code_ << "} catch (std::exception & ex) {";
    code_ << "std::cerr << \"++++++++++ <THEKLA ERROR> \" << std::endl;";
    code_ << "std::cerr << ex.what();";
    code_ << "std::cerr << \"++++++++++ </THEKLA ERROR> \" << std::endl;";
    code_ << "}";
    code_ << "container.show();";
    code_ << "return a.exec();";
    code_ << "}";
    code_ << "";
}
