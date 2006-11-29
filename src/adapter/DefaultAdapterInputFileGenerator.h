
#ifndef THEKLA_DEFAULTADAPTERINPUTFILEGENERATOR_H
#define THEKLA_DEFAULTADAPTERINPUTFILEGENERATOR_H

#include "DefaultAdapterGenerator.h"
#include <QString>

class Log;

//--------------------------------------------------------------------------------
class DefaultAdapterInputFileGenerator
{

private:
    static Log cLog;

public:
    static void exec(QString qObjectClassListFilePath = "QObjectClassList.raw",
                     QString qObjectIncludeFilePath = QOBJECTCLASSLIST_INCLUDEFILE,
                     QString qObjectMacroFilePath = QOBJECTCLASSLIST_MACROFILE);
private:

    DefaultAdapterInputFileGenerator();

    ~DefaultAdapterInputFileGenerator();
};

#endif // THEKLA_DEFAULTADAPTERINPUTFILEGENERATOR_H
