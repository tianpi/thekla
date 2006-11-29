
#### Thekla
INCLUDEPATH += $(THEKLAROOT)/src
LIBS += -L$(THEKLAROOT)/lib

#### Muddleware
INCLUDEPATH += $(MUDDLEWAREROOT)/include
unix {
    LIBS += -L$(MUDDLEWAREROOT)/lib -lXMLClient
}
win32 {
    LIBS += -L$(MUDDLEWAREROOT)/lib/win32
    CONFIG(debug, debug|release) {
        LIBS += -lXMLClientd
    } else {
        LIBS += -lXMLClient
    }
}

#### Log4CPlus
INCLUDEPATH += $(LOG4CPLUSROOT)/include
unix {
    LIBS += -L$(LOG4CPLUSROOT)/src/.libs -llog4cplus
}
win32 {
	DEFINES -= UNICODE
    LIBS += -L$(LOG4CPLUSROOT)/lib
    CONFIG(debug, debug|release) {
        LIBS += -llog4cplusd
    } else {
        LIBS += -llog4cplus
    }
}

#### Coin (from coin-config)
DEFINES += _REENTRANT
unix {
    exists($(COINROOT)) {
        INCLUDEPATH += $(COINROOT)/include
        LIBS += -L$(COINROOT)/lib
    }

    INCLUDEPATH += /usr/include/Inventor/annex /usr/X11R6/include
    LIBS += -L/usr/X11R6/lib -lCoin -lGL  -lXext  -lSM -lICE -lX11  -ldl -lpthread  -lm
}
win32 {
    DEFINES += COIN_NOT_DLL
    INCLUDEPATH += $(COINROOT)/include
    LIBS += -L$(COINROOT)/lib
    CONFIG(debug, debug|release) {
        LIBS += -lCoin2d
    } else {
        LIBS += -lCoin2
    }
}

#### ACE (do we need this??)
unix {
    exists($(ACEROOT)) {
        INCLUDEPATH += $(ACEROOT)
#        LIBS += -L$(ACEROOT)/lib
    }
#    LIBS += -lAce
}
win32 {
    ## The TryCriticalSection thing
    DEFINES += _WIN32_WINNT=0x0400
    INCLUDEPATH += $(ACEROOT)
#    LIBS += -L$(ACEROOT)/lib
#    CONFIG(debug, debug|release) {
#        LIBS += -lAced
#    } else {
#        LIBS += -lAce
#    }
}
