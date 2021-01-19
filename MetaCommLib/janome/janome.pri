QT += serialport network gui

DEFINES += DOBOTDLL_LIBRARY

INCLUDEPATH += $$PWD/include/
DEPENDPATH += $$PWD/include/

SOURCES += $$PWD/src/Janome.cpp \
            $$PWD/src/JanomeSocket.cpp

headers = $$PWD/include/metacommlib/janome/Janome.h \
          $$PWD/include/metacommlib/janome/JanomeSocket.h

HEADERS += $$headers \

#FORMS += $$PWD/src/DobotControlForm.ui

# Install
janome.files = $$headers

win32 {
    janome.path = $$(MTVL_INSTALL)/include/metacommlib/janome/
}

linux:!android {
    janome.path = $$(MTVL_INSTALL)/include/metacommlib/janome/
}

INSTALLS += janome
