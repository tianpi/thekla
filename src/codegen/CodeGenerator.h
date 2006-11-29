
#ifndef THEKLA_CODEGENERATOR_H
#define THEKLA_CODEGENERATOR_H

#include <common/config-win32.h>

#include <QString>
#include <QStringList>
#include <iostream>

//--------------------------------------------------------------------------------
class THEKLA_EXPORT CodeGenerator
{
public:

    static void saveToFile(const QString & path, const QString & contents);

protected:

    QStringList code_;

protected:

    CodeGenerator();
    virtual ~CodeGenerator()
        {};

protected:

    virtual void preProcess() = 0;

    virtual void generateCode() = 0;

public:

    void exec();

    /**
     * Returns the generated source code.
     */
    QString getCode() const;
};


#endif // THEKLA_CODEGENERATOR_H
