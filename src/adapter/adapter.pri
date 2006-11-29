#### Sources

HEADERS.adapter = \
    QtAdapter.h \
    QtAdapterFactory.h

SOURCES.adapter = \
    QtAdapter.cpp \
    QtAdapterFactory.cpp

LIBS += $$LIBDIR/libTheklaQtAdapters.a

for(a,HEADERS.adapter) {  HEADERS += $(THEKLAROOT)/src/adapter/$${a} }
for(a,SOURCES.adapter) {  SOURCES += $(THEKLAROOT)/src/adapter/$${a} }
for(a,HEADERS.adapter) {  HEADERS_INSTALL.adapter += adapter/$${a} }

#### Installation

headers.adapter.files = $$HEADERS_INSTALL.adapter
headers.adapter.path = $$INSTALL_INCDIR/adapter
INSTALLS += headers.adapter
