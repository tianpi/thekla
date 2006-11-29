#### Sources

HEADERS.xml = \
    XmlSubDocument.h \ 
    XmlFactory.h \
    XmlDocument.h \
    XmlDatabaseDocument.h \
    XmlConfigFileDocument.h \
    XmlElementData.h \
    XmlElementInfo.h \
    XmlElement.h \
    DBRootElement.h \
    UIDataRoot.h \
    UISettings.h \
    QtObject.h \
    QtSignal.h \
    QtProperty.h \
    OivField.h \
    XmlTrashDocument.h

SOURCES.xml = \
    XmlElementInfo.cpp \
    XmlSubDocument.cpp \
    XmlFactory.cpp \
    XmlDocument.cpp \
    XmlElementData.cpp \
    XmlElement.cpp \
    XmlDatabaseDocument.cpp \
    XmlConfigFileDocument.cpp \
    UIDataRoot.cpp \
    UISettings.cpp \
    QtObject.cpp \
    QtSignal.cpp \
    QtProperty.cpp \
    OivField.cpp \
    XmlTrashDocument.cpp

for(a,HEADERS.xml) {  HEADERS += $(THEKLAROOT)/src/xml/$${a} }
for(a,SOURCES.xml) {  SOURCES += $(THEKLAROOT)/src/xml/$${a} }
for(a,HEADERS.xml) {  HEADERS_INSTALL.xml += xml/$${a} }

#### Installation

headers.xml.files = $$HEADERS_INSTALL.xml
headers.xml.path = $$INSTALL_INCDIR/xml
INSTALLS += headers.xml
