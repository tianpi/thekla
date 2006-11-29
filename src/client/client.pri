#### Sources

HEADERS.client = \
    Client.h

SOURCES.client = \
    Client.cpp

for(a,HEADERS.client) {  HEADERS += $(THEKLAROOT)/src/client/$${a} }
for(a,SOURCES.client) {  SOURCES += $(THEKLAROOT)/src/client/$${a} }
for(a,HEADERS.client) {  HEADERS_INSTALL.client += client/$${a} }

#### Installation

headers.client.files = $$HEADERS_INSTALL.client
headers.client.path = $$INSTALL_INCDIR/client
INSTALLS += headers.client
