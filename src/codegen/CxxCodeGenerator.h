
#ifndef THEKLA_CXXCODEGENERATOR_H
#define THEKLA_CXXCODEGENERATOR_H

#include "CodeGenerator.h"

//--------------------------------------------------------------------------------
class THEKLA_EXPORT CxxCodeGenerator : public CodeGenerator
{
protected:

    QString className_;
    QString baseClassName_;

    QList<QString> localIncludeList_;
    QList<QString> globalIncludeList_;
    QList<QString> forwardDeclList_;

protected:
    CxxCodeGenerator();
    virtual ~CxxCodeGenerator()
        {};

protected:

    virtual void codeHeaderHead();

    virtual void codeHeaderFoot();

    virtual void codeLocalIncludes();

    virtual void codeGlobalIncludes();

    virtual void codeForwardDecls();

    virtual void codeClassDecl();

public:

    /**
     * Returns the QtAdapter header filepath.
     */
    QString getClassName() const
        { return className_; };
};


#endif // THEKLA_CXXCODEGENERATOR_H
