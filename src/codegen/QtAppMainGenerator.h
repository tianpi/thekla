
#ifndef THEKLA_QTAPPMAINGENERATOR_H
#define THEKLA_QTAPPMAINGENERATOR_H

#include "UiClassWrapperGenerator.h"

class XmlConfigFileDocument;
class UIDataRoot;
class UISettings;

//--------------------------------------------------------------------------------
class THEKLA_EXPORT QtAppMainGenerator : public UiClassWrapperGenerator
{
private:
    QtAppMainGenerator();
public:
    QtAppMainGenerator(const XmlConfigFileDocument & fileDoc);
    ~QtAppMainGenerator()
        {};

private:
    virtual void preProcess();

    virtual void generateCode();
};


#endif // THEKLA_QTAPPMAINGENERATOR_H
