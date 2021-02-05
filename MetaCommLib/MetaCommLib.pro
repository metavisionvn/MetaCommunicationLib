QT += gui core

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = lib
DEFINES += METACOMMLIB_LIBRARY

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# EIGEN Library Add Eigen Lib to Path of Windows.
INCLUDEPATH += $$PWD/include

# include subfolders
include(dobot/dobot.pri)
include(janome/janome.pri)
include(techman/techman.pri)
# end
DestinationDir = $$PWD/../build

SOURCES += \
            IRobot.cpp \
            IRobotFactory.cpp \
            IRobotPosition.cpp \
            IRobotUserControl.cpp \
            TcpClient.cpp \
            TcpServer.cpp \
            TcpSocketBase.cpp \
            metacommlib.cpp

headers = \
        $$PWD/include/metacommlib/MetaCommLib_global.h \
        $$PWD/include/metacommlib/IRobot.h \
        $$PWD/include/metacommlib/IRobotPosition.h \
        $$PWD/include/metacommlib/metacommlib.h \
        $$PWD/include/metacommlib/IRobotUserControl.h \
        $$PWD/include/metacommlib/IRobotFactory.h \
        $$PWD/include/metacommlib/CommLibDefines.h \
        $$PWD/include/metacommlib/TcpClient.h \
        $$PWD/include/metacommlib/TcpServer.h \
        $$PWD/include/metacommlib/TcpSocketBase.h

HEADERS += $$headers

win32 {
    message("Windows Configuration")
    # Destination directory configuration
    CONFIG(debug, debug|release):DestinationDir = $$DestinationDir/win32/debug
    CONFIG(release, debug|release): DestinationDir = $$DestinationDir/win32/release
    DESTDIR = $$DestinationDir

    global.files = $$DestinationDir/*.*
    global.path = $$(MTVL_INSTALL)/lib
    INSTALLS += global

    headers.files = $$headers
    headers.path = $$(MTVL_INSTALL)/include/metacommlib
    INSTALLS += headers
}

# Default rules for deployment.
linux:!android {
    message("Linux Configuration")
    QMAKE_CXXFLAGS += -fopenmp
    QMAKE_LFLAGS +=  -fopenmp
    QMAKE_CXXFLAGS += -O3
    QMAKE_CFLAGS += -O3
    # Build up for arm processor. Enable SIMD
    equals(QT_ARCH, arm) {
        message("ARM Processor")
        QMAKE_CXXFLAGS_RELEASE += -mfpu=neon
        QMAKE_CFLAGS_RELEASE += -mfpu=neon

    }
    # Destination directory configuration
    CONFIG(debug, debug|release):DestinationDir = $$DestinationDir/unix/debug
    CONFIG(release, debug|release): DestinationDir = $$DestinationDir/unix/release
    DESTDIR = $$DestinationDir

    global.files = $$DestinationDir/*
    global.path = $$(MTVL_INSTALL)/lib
    INSTALLS += global

    headers.files = $$headers
    headers.path = $$(MTVL_INSTALL)/include/metacommlib
    INSTALLS += headers
}

OBJECTS_DIR = $$PWD/../build/obj
MOC_DIR = $$PWD/../build/moc
RCC_DIR = $$PWD/../build/qrc
UI_DIR = $$PWD/../build/ui

#!isEmpty(target.path): INSTALLS += target
#INSTALLS += target
