
#### Global project settings

include($$(THEKLAROOT)/thekla-global.pri)
include($(THEKLAROOT)/thekla-deps.pri)

#### Project settings

TEMPLATE = lib
TARGET = TheklaQtAdapters
DESTDIR = $$LIBDIR
message("DESTDIR = $$DESTDIR")

CONFIG += staticlib

CONFIG(debug, debug|release) {
    win32: TARGET = $$join(TARGET,,d)
//    else: TARGET = $$join(TARGET,,,_debug)
}

#### Sources

include(QtAdapterClassList.pri)
