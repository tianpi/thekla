
#include "QtAppProjectGenerator.h"

#include <xml/XmlConfigFileDocument.h>
#include <xml/UIDataRoot.h>
#include <xml/UISettings.h>
#include <common/QDPluginUtils.h>

//--------------------------------------------------------------------------------
QtAppProjectGenerator::QtAppProjectGenerator(const XmlConfigFileDocument & fileDoc)
    : UiClassWrapperGenerator(fileDoc)
{
}

//--------------------------------------------------------------------------------
void QtAppProjectGenerator::preProcess()
{
}

//--------------------------------------------------------------------------------
void QtAppProjectGenerator::generateCode()
{
    code_ << "include($$(THEKLAROOT)/thekla-app.pri)";
    code_ << "";
    code_ << "TEMPLATE = app";
    code_ << "TARGET =";
    code_ << "DESTDIR= .";
    code_ << "";
    code_ << "FORMS += \\";
    code_ << "\t" + QDPluginUtils::getFileName(uiSettings_->getUiFileName());
    code_ << "";
    code_ << "HEADERS += \\";
    QString uiWrapperFilePath = QDPluginUtils::getUiWrapperFilePath(uiSettings_->getUiFileName());
    code_ << "\t" + QDPluginUtils::getFileName(uiWrapperFilePath);
    code_ << "";
    code_ << "SOURCES += \\";
    code_ << "\tmain.cpp";
    code_ << "";
}
