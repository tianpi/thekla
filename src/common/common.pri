#### Sources

HEADERS.common = \
    config-win32.h \
    Base64.h \
    DatabaseConn.h \
    Exception.h \
    Log.h \
    Observable.h \
    Observer.h \
    QDPluginUtils.h \
    QtAdapterUtils.h \
    QtObjectUtils.h \
    TypeConverter.h \
    Variant.h \
    DatabaseConnManager.h \
    DatabaseEventDispatcher.h

SOURCES.common = \
    Base64.cpp \
    DatabaseConn.cpp \
    Exception.cpp \
    Log.cpp \
    Observable.cpp \
    Observer.cpp \
    QDPluginUtils.cpp \
    QtObjectUtils.cpp \
    TypeConverter.cpp \
    Variant.cpp \
    DatabaseConnManager.cpp \
    DatabaseEventDispatcher.cpp

for(a,HEADERS.common) {  HEADERS += $(THEKLAROOT)/src/common/$${a} }
for(a,SOURCES.common) {  SOURCES += $(THEKLAROOT)/src/common/$${a} }
for(a,HEADERS.common) {  HEADERS_INSTALL.common += common/$${a} }

#### Installation

headers.common.files = $$HEADERS_INSTALL.common
headers.common.path = $$INSTALL_INCDIR/common
INSTALLS += headers.common
