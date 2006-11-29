
#ifndef THEKLA_XMLCONFIGFILEDOCUMENT_H
#define THEKLA_XMLCONFIGFILEDOCUMENT_H

#include "XmlDocument.h"

#include <QFile>

class Log;
class UISettings;
class UIDataRoot;

//--------------------------------------------------------------------------------
class THEKLA_EXPORT XmlConfigFileDocument : public XmlDocument
{
private:
    static Log cLog;

private:
    XmlConfigFileDocument(const XmlConfigFileDocument & copy);

public:
    XmlConfigFileDocument();
    virtual ~XmlConfigFileDocument();

public:

    void init();

    void load(QFile & inputFile);

    void save(QFile & outputFile);

    UISettings*  getUISettings() const;

    UIDataRoot* getUIDataRoot() const;

};

#endif // THEKLA_XMLCONFIGFILEDOCUMENT_H
