#### Global project settings

include($$(THEKLAROOT)/thekla-global.pri)
include($$(THEKLAROOT)/thekla-deps.pri)
include($$(THEKLAROOT)/thekla.pri)

#### Project settings

TEMPLATE = app
TARGET = 
DESTDIR = .
message("DESTDIR = $$DESTDIR")

#### Sources

HEADERS += \
    DefaultAdapterGenerator.h

SOURCES += \
    DefaultAdapterGenerator.cpp \
    main_default-adapter-generator.cpp
