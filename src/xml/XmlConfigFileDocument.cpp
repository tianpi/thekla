
#include "XmlConfigFileDocument.h"
#include "UIDataRoot.h"
#include "UISettings.h"

#include <common/Log.h>
#include <common/Exception.h>

#include <QIODevice>

//--------------------------------------------------------------------------------
Log XmlConfigFileDocument::cLog(Log::getClassLog("XmlConfigFileDocument"));

//--------------------------------------------------------------------------------
XmlConfigFileDocument::XmlConfigFileDocument()
    : XmlDocument()
{
}

//--------------------------------------------------------------------------------
XmlConfigFileDocument::~XmlConfigFileDocument()
{

}

//--------------------------------------------------------------------------------
void XmlConfigFileDocument::init()
{
    if (isInitialized_)
        return;

    registerDefaultElements();
    XmlElementInfo * uiDataRootInfo = registerElement("TheklaUIData", new UIDataRoot, NULL);
//    XmlElementInfo * uiSettingsInfo =
        registerElement("Settings", new UISettings, uiDataRootInfo);

    // add THEKLA root element
    UIDataRoot * uiDataRoot = dynamic_cast<UIDataRoot*>(getElement("TheklaUIData"));
    setRootElement(uiDataRoot);

    // add THEKLA settings element
    UISettings * uiSettings = dynamic_cast<UISettings*>(getElement("Settings"));
    uiSettings->setParent(uiDataRoot);

    isInitialized_ = true;
}

//--------------------------------------------------------------------------------
void XmlConfigFileDocument::save(QFile & outputFile)
{
    if (rootElement_ == NULL)
        return;

    if (!outputFile.open(QIODevice::WriteOnly)) {
        THROW_THEKLA_EXCEPTION("Cannot open file (" << outputFile.fileName() << ") for writing.");
    }

    QDomDocument tmp = rootElement_->getDom();
    QDomDocument doc("Thekla");
    doc.appendChild(tmp.documentElement());

    if ((outputFile.write(doc.toByteArray()) == -1) || !outputFile.flush())
    {
        THROW_THEKLA_EXCEPTION("Error writing config file (" << outputFile.fileName() << ").");
    }
    outputFile.close();
}

//--------------------------------------------------------------------------------
void XmlConfigFileDocument::load(QFile & inputFile)
{
    Log log(Log::getMethodLog(cLog, "load()"));

    // open the file
    if (!inputFile.open(QIODevice::ReadOnly)) {
        THROW_THEKLA_EXCEPTION("Cannot open file (" << inputFile.fileName() << ") for reading.");
    }

    // read the file to XML DOM document
    QDomDocument doc = XmlDocument::parse(inputFile);

    // flush the current DOM contents (!)
    if (rootElement_ != NULL) {
        THEKLA_DEBUG(log, "Flushing current database contents.");
        delete rootElement_;
    }


    // convert to our DOM representation
    XmlSubDocument subDoc = createSubDocument(doc);
    setRootElement(subDoc.getRootElement());

    inputFile.close();
}

//--------------------------------------------------------------------------------
UISettings * XmlConfigFileDocument::getUISettings() const
{
    XmlElement * xmlElem = findFirstElement(QString("Settings"));
    UISettings * ret = dynamic_cast<UISettings*>(xmlElem);
    if (ret == NULL) {
        THROW_THEKLA_EXCEPTION("UISettings Element has NULL pointer.");
    }
    return ret;
}

//--------------------------------------------------------------------------------
UIDataRoot* XmlConfigFileDocument::getUIDataRoot() const
{
    XmlElement * xmlElem = getRootElement();
    UIDataRoot * ret = dynamic_cast<UIDataRoot*>(xmlElem);
    if (ret == NULL) {
        THROW_THEKLA_EXCEPTION("UIDataRoot Element has NULL pointer.");
    }
    return ret;
}

