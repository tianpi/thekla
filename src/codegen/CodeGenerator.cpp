
#include "CodeGenerator.h"
#include <common/Exception.h>
#include <QFile>


//--------------------------------------------------------------------------------
void CodeGenerator::saveToFile(const QString & path, const QString & contents)
{
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly)) {
        THROW_THEKLA_EXCEPTION("Cannot open file path (" << path << ") for writing.");
    }
    file.write(contents.toAscii());
    file.flush();
    file.close();
}

//--------------------------------------------------------------------------------
CodeGenerator::CodeGenerator()
    : code_()
{
}

//--------------------------------------------------------------------------------
void CodeGenerator::exec()
{
    preProcess();
    generateCode();
}

//--------------------------------------------------------------------------------
QString CodeGenerator::getCode() const
{
    return code_.join("\n");
}

