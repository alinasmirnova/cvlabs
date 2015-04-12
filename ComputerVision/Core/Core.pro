#-------------------------------------------------
#
# Project created by QtCreator 2015-02-28T14:04:12
#
#-------------------------------------------------

TARGET = Core
TEMPLATE = lib
CONFIG += staticlib
CONFIG += c++1y
QMAKE_CXXFLAGS += -std=c++1y

SOURCES += image.cpp \
    filtermanager.cpp \
    maskfactory.cpp \
    mask.cpp \
    separatedmask.cpp \
    pyramid.cpp \
    pyramidlevel.cpp \
    detectors.cpp \
    descriptorgenerator.cpp \
    descriptor.cpp

HEADERS += image.h \
    filtermanager.h \
    maskfactory.h \
    mask.h \
    core.h \
    separatedmask.h \
    pyramid.h \
    pyramidlevel.h \
    detectors.h \
    descriptorgenerator.h \
    descriptor.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}
