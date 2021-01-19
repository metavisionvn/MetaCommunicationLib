QT += serialport network gui

INCLUDEPATH += $$PWD/include/
DEPENDPATH += $$PWD/include/

include(src/ComPlatform/ComPlatform.pri)
include(src/DobotDevice/DobotDevice.pri)

SOURCES += $$PWD/src/DobotDll.cpp \
           $$PWD/src/DobotFactory.cpp \
           $$PWD/src/Dobot.cpp \
           $$PWD/src/DobotPosition.cpp \
           $$PWD/src/CDobot.cpp \
           $$PWD/src/CDobotConnector.cpp \
           $$PWD/src/CDobotProtocol.cpp \
           $$PWD/src/CDobotCommunicator.cpp \
           $$PWD/src/DobotControlForm.cpp \

HEADERS += $$PWD/src/CDobot.h \
           $$PWD/src/CDobotConnector.h \
           $$PWD/src/CDobotProtocol.h \
           $$PWD/src/CDobotCommunicator.h \

FORMS += $$PWD/src/DobotControlForm.ui

headers = $$PWD/include/metacommlib/dobot/dobotdll_global.h \
           $$PWD/include/metacommlib/dobot/DobotType.h\
           $$PWD/include/metacommlib/dobot/DobotDll.h \
           $$PWD/include/metacommlib/dobot/Dobot.h \
           $$PWD/include/metacommlib/dobot/DobotPosition.h \
           $$PWD/include/metacommlib/dobot/DobotFactory.h \
           $$PWD/include/metacommlib/dobot/DobotControlForm.h \

# Install
dobot.files = $$headers

win32 {
    dobot.path = $$(MTVL_INSTALL)/include/metacommlib/dobot/
}

android {
    dobot.path = $$[MTVL_INSTALL_ANDROID]/include/metacommlib/dobot/
}

linux:!android {
    dobot.path = $$(MTVL_INSTALL)/include/metacommlib/dobot/
}

INSTALLS += dobot

