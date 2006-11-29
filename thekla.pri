 
## Thekla
INCLUDEPATH += $(THEKLAROOT)/include/Thekla $(THEKLAROOT)/src
LIBS += -L$(THEKLAROOT)/lib
unix {
    LIBS += -lThekla
}
win32 {
    CONFIG(debug, debug|release) {
        LIBS += -lTheklad 
    } else {
        LIBS += -lThekla
    }
}
