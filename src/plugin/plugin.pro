
#### Global project settings

include($$(THEKLAROOT)/thekla-global.pri)
include($(THEKLAROOT)/thekla-deps.pri)

#### Additional deps

include($$(THEKLAROOT)/thekla.pri)

#### Project settings

TEMPLATE = lib
CONFIG  += designer plugin
TARGET = TheklaQDPlugin
DESTDIR  = $$LIBDIR
message(DESTDIR = $$DESTDIR)

CONFIG(debug, debug|release) {
    win32: TARGET = $$join(TARGET,,d)
    else: TARGET = $$join(TARGET,,,_debug)
}

#### Sources

## QtObjectSettingsDialog
FORMS += \
    QtObjectSettingsDialog.ui \
    QtObjectClassSelectionWidget.ui \
    QtSignalSettingsWidget.ui \
    QtPropertySettingsWidget.ui

## SettingsDialog
FORMS += \
    GlobalSettingsDialog.ui

HEADERS += \
    QDPlugin.h \
    QDWidget.h \
    QDTaskMenu.h \
    QDFormManager.h \
    QDFormWindowManager.h \
    QDFormWindowUiFileObserver.h \
    QtObjectSettingsDialog.h \
    GlobalSettingsDialog.h 

SOURCES +=  \
    QDPlugin.cpp \
    QDWidget.cpp \
    QDTaskMenu.cpp \
    QDFormManager.cpp \
    QDFormWindowManager.cpp \
    QDFormWindowUiFileObserver.cpp \
    QtObjectSettingsDialog.cpp \
    GlobalSettingsDialog.cpp 

## Installation

target.path = $$[QT_INSTALL_PLUGINS]/designer
##sources.files = $$SOURCES $$HEADERS *.pro
##sources.path = $$[QT_INSTALL_EXAMPLES]/designer/taskmenuextension
INSTALLS += target
