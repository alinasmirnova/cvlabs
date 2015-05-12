TEMPLATE = subdirs

SUBDIRS += \
    Core \
    Labs
Labs.depends = Core

INCLUDEPATH += $$PWD/../../../GnuWin32/include
DEPENDPATH += $$PWD/../../../GnuWin32/include

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../GnuWin32/lib/ -lgsl
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../GnuWin32/lib/ -lgsld
else:unix: LIBS += -L$$PWD/../../GnuWin32/lib/ -lgsl

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../GnuWin32/lib/ -lgslcblas
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../GnuWin32/lib/ -lgslcblasd
else:unix: LIBS += -L$$PWD/../../GnuWin32/lib/ -lgslcblas

