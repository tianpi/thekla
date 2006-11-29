
#### Configuration

CONFIG += qt warn_on
unix {
    CONFIG += release
}
win32 {
    CONFIG += debug_and_release
}
QT += xml

VERSION = 1.0.0
 
#CONFIG += precompile_header
#PRECOMPILED_HEADER = $(THEKLAROOT)/src/adapter/QtAdapterClassList.h
#HEADERS += $(THEKLAROOT)/src/adapter/QtAdapterClassList.h

#### Common path settings

BASEDIR    = $$(THEKLAROOT)
SRCDIR     = $$BASEDIR/src
INCLUDEDIR = $$BASEDIR/include
LIBDIR     = $$BASEDIR/lib
BINDIR     = $$BASEDIR/bin

message("BASEDIR = $$BASEDIR")
message("SRCDIR = $$SRCDIR")
message("INCLUDEDIR = $$INCLUDEDIR")
message("LIBDIR = $$LIBDIR")

unix {

    # NOTE: change this according to your needs
    INSTALL_PREFIX = /tmp

    INSTALL_LIBDIR = $$INSTALL_PREFIX/lib
    INSTALL_INCDIR = $$INSTALL_PREFIX/include/Thekla
}
message("INSTALL_PREFIX = $$INSTALL_PREFIX")

