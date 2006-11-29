
#ifndef THEKLA_QTAPPPROJECTGENERATOR_H
#define THEKLA_QTAPPPROJECTGENERATOR_H

#include "UiClassWrapperGenerator.h"

class XmlConfigFileDocument;
class UIDataRoot;
class UISettings;

//--------------------------------------------------------------------------------
class THEKLA_EXPORT QtAppProjectGenerator : public UiClassWrapperGenerator
{
private:
    QtAppProjectGenerator();
public:
    QtAppProjectGenerator(const XmlConfigFileDocument & fileDoc);
    ~QtAppProjectGenerator()
        {};

private:
    virtual void preProcess();

    virtual void generateCode();
};


#endif // THEKLA_QTAPPPROJECTGENERATOR_H
