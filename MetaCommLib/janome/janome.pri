QT += serialport network gui

DEFINES += DOBOTDLL_LIBRARY

INCLUDEPATH += $$PWD/include/
DEPENDPATH += $$PWD/include/

SOURCES += $$PWD/src/Janome.cpp \
            $$PWD/src/JanomeFactory.cpp \
            $$PWD/src/JanomeControlForm.cpp \
            $$PWD/src/JanomePosition.cpp \
            $$PWD/src/JanomeDecodeMsg.cpp \
            $$PWD/src/JanomeSocket.cpp

headers = $$PWD/include/metacommlib/janome/Janome.h \
          $$PWD/include/metacommlib/janome/JanomeSocket.h \
          $$PWD/include/metacommlib/janome/JanomeDecodeMsg.h \
          $$PWD/include/metacommlib/janome/JanomePosition.h \
          $$PWD/include/metacommlib/janome/JanomeControlForm.h \
          $$PWD/include/metacommlib/janome/JanomeFactory.h

HEADERS += $$headers

FORMS += $$PWD/src/JanomeControlForm.ui

# Install
janome.files = $$headers

win32 {
    janome.path = $$(MTVL_INSTALL)/include/metacommlib/janome/
}

linux:!android {
    janome.path = $$(MTVL_INSTALL)/include/metacommlib/janome/
}

INSTALLS += janome
