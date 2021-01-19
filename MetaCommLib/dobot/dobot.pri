QT += serialport network gui

DEFINES += DOBOTDLL_LIBRARY

INCLUDEPATH += $$PWD/include/
DEPENDPATH += $$PWD/include/

SOURCES += $$PWD/src/DobotDll.cpp \
           $$PWD/src/DobotFactory.cpp \
           $$PWD/src/Dobot.cpp \
           $$PWD/src/DobotPosition.cpp \
           $$PWD/src/CDobot.cpp \
           $$PWD/src/CDobotConnector.cpp \
           $$PWD/src/CDobotProtocol.cpp \
           $$PWD/src/CDobotCommunicator.cpp \
           $$PWD/src/DobotControlForm.cpp \
           $$PWD/src/ComPlatform/Message.cpp \
           $$PWD/src/ComPlatform/Packet.cpp \
           $$PWD/src/ComPlatform/RingBuffer.cpp \
           $$PWD/src/DobotDevice/UdpPort.cpp \
           $$PWD/src/DobotDevice/UdpSearch.cpp

headers = $$PWD/include/metacommlib/dobot/CDobot.h \
          $$PWD/include/metacommlib/dobot/CDobotConnector.h \
          $$PWD/include/metacommlib/dobot/CDobotProtocol.h \
          $$PWD/include/metacommlib/dobot/CDobotCommunicator.h \
          $$PWD/include/metacommlib/dobot/dobotdll_global.h \
          $$PWD/include/metacommlib/dobot/DobotType.h\
          $$PWD/include/metacommlib/dobot/DobotDll.h \
          $$PWD/include/metacommlib/dobot/Dobot.h \
          $$PWD/include/metacommlib/dobot/DobotPosition.h \
          $$PWD/include/metacommlib/dobot/DobotFactory.h \
          $$PWD/include/metacommlib/dobot/DobotControlForm.h

complatform_headers = $$PWD/include/metacommlib/dobot/ComPlatform/Message.h \
                      $$PWD/include/metacommlib/dobot/ComPlatform/Packet.h \
                      $$PWD/include/metacommlib/dobot/ComPlatform/RingBuffer.h \
                      $$PWD/include/metacommlib/dobot/ComPlatform/ProtocolDef.h \
                      $$PWD/include/metacommlib/dobot/ComPlatform/ProtocolID.h

dobotdevice_headers = $$PWD/include/metacommlib/dobot/DobotDevice/UdpPort.h \
                      $$PWD/include/metacommlib/dobot/DobotDevice/UdpSearch.h

HEADERS += $$headers \
            $$complatform_headers \
            $$dobotdevice_headers

FORMS += $$PWD/src/DobotControlForm.ui

# Install
dobot.files = $$headers
complatform.files = $$complatform_headers
dobotdevice.files = $$dobotdevice_headers

win32 {
    dobot.path = $$(MTVL_INSTALL)/include/metacommlib/dobot/
    complatform.path = $$(MTVL_INSTALL)/include/metacommlib/dobot/ComPlatform
    dobotdevice.path = $$(MTVL_INSTALL)/include/metacommlib/dobot/DobotDevice
}

linux:!android {
    dobot.path = $$(MTVL_INSTALL)/include/metacommlib/dobot/
    complatform.path = $$(MTVL_INSTALL)/include/metacommlib/dobot/ComPlatform
    dobotdevice.path = $$(MTVL_INSTALL)/include/metacommlib/dobot/DobotDevice
}

INSTALLS += dobot complatform dobotdevice

