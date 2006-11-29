#### Sources

HEADERS.codegen = \
    CodeGenerator.h \
    CxxCodeGenerator.h \
    QtAdapterGenerator.h  \
    UiClassWrapperGenerator.h \
    QtAppMainGenerator.h \
    QtAppProjectGenerator.h

SOURCES.codegen = \
    CodeGenerator.cpp \
    CxxCodeGenerator.cpp \
    QtAdapterGenerator.cpp \
    UiClassWrapperGenerator.cpp \
    QtAppMainGenerator.cpp \
    QtAppProjectGenerator.cpp

for(a,HEADERS.codegen) {  HEADERS += $(THEKLAROOT)/src/codegen/$${a} }
for(a,SOURCES.codegen) {  SOURCES += $(THEKLAROOT)/src/codegen/$${a} }
for(a,HEADERS.codegen) {  HEADERS_INSTALL.codegen += codegen/$${a} }

#### Installation

headers.codegen.files = $$HEADERS_INSTALL.codegen
headers.codegen.path = $$INSTALL_INCDIR/codegen
INSTALLS += headers.codegen

