
#### Configuration

CONFIG += qt warn_on
unix {
    CONFIG += release
}
win32 {
    CONFIG += debug_and_release
}
QT += xml


#### Dependencies

include($(THEKLAROOT)/thekla-deps.pri)
include($(THEKLAROOT)/thekla.pri)
