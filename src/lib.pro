
#### Global project settings

include($$(THEKLAROOT)/thekla-global.pri)
include($(THEKLAROOT)/thekla-deps.pri)

#### Project settings

TEMPLATE = lib
TARGET = Thekla
DESTDIR= $$LIBDIR
message("DESTDIR = $$DESTDIR")

CONFIG += qt
QT += xml

CONFIG(debug, debug|release) {
    win32: TARGET = $$join(TARGET,,d)
//    else: TARGET = $$join(TARGET,,,_debug)
}

OBJECTS_DIR = .objs
MOC_DIR = .moc
RCC_DIR = .rcc
UI_DIR = .ui
UI_HEADERS_DIR = .
UI_SOURCES_DIR = .

#### Sources

## include sub-project files
include(common/common.pri)
include(adapter/adapter.pri)
include(codegen/codegen.pri)
include(xml/xml.pri)
include(client/client.pri)

#### Installation

target.path = $$INSTALL_LIBDIR
INSTALLS += target
