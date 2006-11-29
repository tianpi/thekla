
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
    DefaultAdapterInputFileGenerator.h

SOURCES += \
    DefaultAdapterInputFileGenerator.cpp \
    main_default-adapter-input-file-generator.cpp
