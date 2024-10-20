QT       += core gui
QT       += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

QMAKE_RESOURCE_FLAGS += -compress 2 -threshold 3
# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/scena_sonaru.cpp \
    src/daneczujnika.cpp \
    src/main.cpp \
    src/mainwindow.cpp

HEADERS += \
    inc/daneczujnika.hh \
    inc/mainwindow.hh \
    inc/scena_sonaru.hh 

FORMS += \
    forms/mainwindow.ui

TRANSLATIONS += \
    inne/lang/lang.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    inne/lang/lang.qm \
    inne/lang/lang.ts \
    inne/res/dioda/czerwona.png \
    inne/res/dioda/zielona.png \
    inne/res/serwo/serwo0.png \
    inne/res/serwo/serwo10.png \
    inne/res/serwo/serwo15.png \
    inne/res/serwo/serwo20.png \
    inne/res/serwo/serwo25.png \
    inne/res/serwo/serwo30.png \
    inne/res/serwo/serwo35.png \
    inne/res/serwo/serwo40.png \
    inne/res/serwo/serwo45.png \
    inne/res/serwo/serwo5.png \
    inne/res/serwo/serwo50.png \
    inne/res/serwo/serwo55.png \
    inne/res/serwo/serwo60.png \
    inne/res/serwo/serwo65.png \
    inne/res/serwo/serwo70.png \
    inne/res/serwo/serwo75.png \
    inne/res/serwo/serwo80.png \
    inne/res/serwo/serwo85.png \
    inne/res/serwo/serwo90.png

RESOURCES += \
    zasoby.qrc

TRANSLATIONS = inne/lang/lang.ts
