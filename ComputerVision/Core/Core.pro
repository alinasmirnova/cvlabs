#-------------------------------------------------
#
# Project created by QtCreator 2015-02-28T14:04:12
#
#-------------------------------------------------

TARGET = Core
TEMPLATE = lib
CONFIG += staticlib
CONFIG += c++11
QMAKE_CXXFLAGS += -std=c++11

SOURCES += image.cpp

HEADERS += image.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}