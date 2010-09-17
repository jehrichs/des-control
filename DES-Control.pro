#-------------------------------------------------
#
# Project created by QtCreator 2010-09-12T23:10:41
#
#-------------------------------------------------

QT       += core gui network xml

TARGET = DES-Control
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    settingsdialog.cpp \
    trainsettings.cpp \
    actuatorsettings.cpp \
    sensorsettings.cpp \
    serversettings.cpp \
    serverdebugconsole.cpp \
    project.cpp \
    dcserver.cpp \
    dcmodelitem.cpp \
    dctrain.cpp \
    dcsensor.cpp \
    dcactuator.cpp \
    projecttreeview.cpp \
    projectserializer.cpp

HEADERS  += mainwindow.h \
    settingsdialog.h \
    trainsettings.h \
    actuatorsettings.h \
    sensorsettings.h \
    serversettings.h \
    serverdebugconsole.h \
    project.h \
    dcserver.h \
    dcmodelitem.h \
    dctrain.h \
    dcsensor.h \
    dcactuator.h \
    projecttreeview.h \
    projectserializer.h

FORMS    += mainwindow.ui \
    settingsdialog.ui \
    trainsettings.ui \
    actuatorsettings.ui \
    sensorsettings.ui \
    serversettings.ui \
    serverdebugconsole.ui

RESOURCES += \
    desIcons.qrc
