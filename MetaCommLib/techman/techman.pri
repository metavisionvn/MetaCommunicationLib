QT += serialport network gui

INCLUDEPATH += $$PWD/include/
DEPENDPATH += $$PWD/include/

SOURCES += $$PWD/src/Techman.cpp  \
    $$PWD/src/TechmanPosition.cpp

headers = $$PWD/include/metacommlib/techman/Techman.h \
            $$PWD/include/metacommlib/techman/TechmanPosition.h

HEADERS += $$headers

# FORMS += $$PWD/src/DobotControlForm.ui

# Install
techman.files = $$headers

win32 {
    techman.path = $$(MTVL_INSTALL)/include/metacommlib/techman/
}

linux:!android {
    techman.path = $$(MTVL_INSTALL)/include/metacommlib/techman/
}

INSTALLS += techman

