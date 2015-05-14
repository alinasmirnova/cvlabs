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
    models.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../GnuWin32/lib/ -llibgsl
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../GnuWin32/lib/ -llibgsl
else:unix: LIBS += -L$$PWD/../../../GnuWin32/lib/ -llibgsl

INCLUDEPATH += $$PWD/../../../GnuWin32/include
DEPENDPATH += $$PWD/../../../GnuWin32/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../GnuWin32/lib/libgsl.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../GnuWin32/lib/libgsl.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../GnuWin32/lib/libgsl.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../GnuWin32/lib/libgsld.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../../GnuWin32/lib/libgsl.a

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../GnuWin32/lib/ -lgslcblas
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../GnuWin32/lib/ -lgslcblas
else:unix: LIBS += -L$$PWD/../../../GnuWin32/lib/ -lgslcblas

INCLUDEPATH += $$PWD/../../../GnuWin32/include
DEPENDPATH += $$PWD/../../../GnuWin32/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../GnuWin32/lib/libgslcblas.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../GnuWin32/lib/libgslcblas.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../GnuWin32/lib/libgslcblas.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../GnuWin32/lib/libgslcblas.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../../GnuWin32/lib/libgslcblas.a
