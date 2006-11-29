
#include "DefaultAdapterInputFileGenerator.h"

#include <common/Log.h>
#include <common/Exception.h>

#include <QFile>
#include <QStringList>
#include <QTextStream>

//--------------------------------------------------------------------------------
Log DefaultAdapterInputFileGenerator::cLog(Log::getClassLog("DefaultAdapterInputFileGenerator"));

//--------------------------------------------------------------------------------
void DefaultAdapterInputFileGenerator::exec(QString qObjectClassListFilePath,
                                            QString qObjectIncludeFilePath,
                                            QString qObjectMacroFilePath)
{
    THEKLA_INFO(coutLog, "Reading input file: " << qObjectClassListFilePath.toStdString());

    QFile inClassListFile(qObjectClassListFilePath);
    if (!inClassListFile.open(QIODevice::ReadOnly)) {
        QString errMsg;
        errMsg += "Cannot open input file \"";
        errMsg += qObjectClassListFilePath;
        errMsg += "\".";
        THROW_THEKLA_EXCEPTION(errMsg);
    }

    QStringList classNameList;

    QTextStream in(&inClassListFile);
    while (!in.atEnd()) {
        classNameList << in.readLine();
    }
    inClassListFile.close();

    THEKLA_INFO(coutLog, "++ Generating: " << qObjectIncludeFilePath.toStdString());
    QFile outIncludeFile(qObjectIncludeFilePath);
    THEKLA_INFO(coutLog, "++ Generating: " << qObjectMacroFilePath.toStdString());
    QFile outMacroFile(qObjectMacroFilePath);

    if (!outIncludeFile.open(QIODevice::WriteOnly)) {
        QString errMsg;
        errMsg += "Cannot open QObject includes output file \"";
        errMsg += qObjectClassListFilePath;
        errMsg += "\".";
        THROW_THEKLA_EXCEPTION(errMsg);
    }
    if (!outMacroFile.open(QIODevice::WriteOnly)) {
        QString errMsg;
        errMsg += "Cannot open QObject macro output file \"";
        errMsg += qObjectMacroFilePath;
        errMsg += "\".";
        THROW_THEKLA_EXCEPTION(errMsg);
    }

    for (int i=0; i < classNameList.size(); i++) {

        QString className = classNameList.value(i);

        if (className.contains("abstract", Qt::CaseInsensitive))
            continue;

        QString include = "#include <" + className + ">\n";
        QString macro = "CREATE_QMETAOBJECT(" + className + ")\n";

        outIncludeFile.write(include.toAscii());
        outMacroFile.write(macro.toAscii());
    }

    outIncludeFile.flush();
    outIncludeFile.close();

    outMacroFile.flush();
    outMacroFile.close();
}
