#-------------------------------------------------
#
# Project created by QtCreator 2015-02-21T14:48:37
#
#-------------------------------------------------

QT       -= gui

TARGET = ComputerVision
TEMPLATE = lib

DEFINES += COMPUTERVISION_LIBRARY

SOURCES += image.cpp

HEADERS += image.h\
        computervision_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
