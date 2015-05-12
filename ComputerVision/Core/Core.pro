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

INCLUDEPATH += $$PWD/../../../GnuWin32/include
DEPENDPATH += $$PWD/../../../GnuWin32/include

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../GnuWin32/lib/ -lgsl
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../GnuWin32/lib/ -lgsld
else:unix: LIBS += -L$$PWD/../../../GnuWin32/lib/ -lgsl

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../GnuWin32/lib/ -lgslcblas
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../GnuWin32/lib/ -lgslcblasd
else:unix: LIBS += -L$$PWD/../../../GnuWin32/lib/ -lgslcblas


SOURCES += image.cpp \
    filtermanager.cpp \
    maskfactory.cpp \
    mask.cpp \
    separatedmask.cpp \
    pyramid.cpp \
    pyramidlevel.cpp \
    detectors.cpp \
    descriptorgenerator.cpp \
    descriptor.cpp \
    models.cpp

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
    descriptor.h \
    point.h \
    models.h \


unix {
    target.path = /usr/lib
    INSTALLS += target
}


message (INCLUDEPATH: $$INCLUDEPATH)
message (LIBS: $$LIBS)







