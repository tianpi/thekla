
#include "CxxCodeGenerator.h"

//--------------------------------------------------------------------------------
CxxCodeGenerator::CxxCodeGenerator()
    : CodeGenerator(),
      className_(),
      baseClassName_(),
      localIncludeList_(),
      globalIncludeList_(),
      forwardDeclList_()
{
}

//--------------------------------------------------------------------------------
void CxxCodeGenerator::codeHeaderHead()
{
    code_ << "#ifndef THEKLA_" + className_.toUpper() + "_H";
    code_ << "#define THEKLA_" + className_.toUpper() + "_H";
}

//--------------------------------------------------------------------------------
void CxxCodeGenerator::codeHeaderFoot()
{
    code_ << "#endif // THEKLA_" + className_.toUpper() + "_H";
}

//--------------------------------------------------------------------------------
void CxxCodeGenerator::codeLocalIncludes()
{
    for (int i=0; i < localIncludeList_.size(); i++)
        code_ << "#include \"" + localIncludeList_.value(i) + "\"";
}

//--------------------------------------------------------------------------------
void CxxCodeGenerator::codeGlobalIncludes()
{
    for (int i=0; i < globalIncludeList_.size(); i++)
        code_ << "#include <" + globalIncludeList_.value(i) + ">";
}


//--------------------------------------------------------------------------------
void CxxCodeGenerator::codeForwardDecls()
{
    for (int i=0; i < forwardDeclList_.size(); i++)
        code_ << "class " + forwardDeclList_.value(i) + ";";
}

//--------------------------------------------------------------------------------
void CxxCodeGenerator::codeClassDecl()
{
    code_ << "class " + className_ + " : public " + baseClassName_;
}


