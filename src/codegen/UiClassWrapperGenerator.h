
#ifndef THEKLA_UICLASSWRAPPERGENERATOR_H
#define THEKLA_UICLASSWRAPPERGENERATOR_H

#include "CxxCodeGenerator.h"

class XmlConfigFileDocument;
class UIDataRoot;
class UISettings;

//--------------------------------------------------------------------------------
class THEKLA_EXPORT UiClassWrapperGenerator : public CxxCodeGenerator
{
protected:
    UIDataRoot * uiDataRoot_;
    UISettings * uiSettings_;

    QString theklaClientInstanceName_;

private:
    UiClassWrapperGenerator();
public:
    UiClassWrapperGenerator(const XmlConfigFileDocument & fileDoc);
    virtual ~UiClassWrapperGenerator()
        {};

private:
    virtual void preProcess();

    virtual void generateCode();

    void codeClassDecl();

    void codeConstructor();

    void codeDestructor();

    void codeSetupUiMethod();

};


#endif // THEKLA_UICLASSWRAPPERGENERATOR_H
