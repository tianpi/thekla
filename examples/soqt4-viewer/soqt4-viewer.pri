
#### THEKLA application settings

include($$(THEKLAROOT)/thekla-app.pri)

#### Additional dependencies

## SoQt4
unix {
    INCLUDEPATH += $(SOQT4ROOT)/include
    LIBS += -L$(SOQT4ROOT)/lib -lSoQt4
}

#### Project settings


#### Sources

HEADERS += \
    SoTheklaNode.h

SOURCES += \
    SoTheklaNode.cpp
