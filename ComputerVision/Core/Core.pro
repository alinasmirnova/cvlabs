#-------------------------------------------------
#
# Project created by QtCreator 2015-02-23T15:45:08
#
#-------------------------------------------------

TARGET = Core
TEMPLATE = lib
CONFIG += staticlib

SOURCES += image.cpp

HEADERS += image.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
