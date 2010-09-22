#-------------------------------------------------
#
# Project created by QtCreator 2010-09-12T23:10:41
#
#-------------------------------------------------

QT       += core gui network xml

TARGET = DES-Control
TEMPLATE = app
TRANSLATIONS = descontroltr_en_US.ts\
               descontroltr_de_DE.ts

LIBS += -L/usr/lib -lgvc

SOURCES += main.cpp\
           mainwindow.cpp \
           settingsdialog.cpp \
           srcp/trainsettings.cpp \
           srcp/actuatorsettings.cpp \
           srcp/sensorsettings.cpp \
           srcp/serversettings.cpp \
           srcp/serverdebugconsole.cpp \
           project.cpp \
           srcp/dcserver.cpp \
           srcp/dcmodelitem.cpp \
           srcp/dctrain.cpp \
           srcp/dcsensor.cpp \
           srcp/dcactuator.cpp \
           projecttreeview.cpp \
           projectserializer.cpp \
           des/dcautomaton.cpp \
           des/dcstate.cpp \
           des/dcevent.cpp \
           des/automatonview.cpp \
           import/importautomaton.cpp \
    des/dctransition.cpp \
    des/gvgraph.cpp \
    srcp/srcpevent.cpp \
    des/dccontroller.cpp

HEADERS  += mainwindow.h \
            settingsdialog.h \
            srcp/trainsettings.h \
            srcp/actuatorsettings.h \
            srcp/sensorsettings.h \
            srcp/serversettings.h \
            srcp/serverdebugconsole.h \
            project.h \
            srcp/dcserver.h \
            srcp/dcmodelitem.h \
            srcp/dctrain.h \
            srcp/dcsensor.h \
            srcp/dcactuator.h \
            projecttreeview.h \
            projectserializer.h \
            des/dcautomaton.h \
            des/dcstate.h \
            des/dcevent.h \
            des/automatonview.h \
            import/importautomaton.h \
    des/dctransition.h \
    des/gvgraph.h \
    srcp/srcpevent.h \
    des/dccontroller.h

FORMS    += mainwindow.ui \
            settingsdialog.ui \
            srcp/trainsettings.ui \
            srcp/actuatorsettings.ui \
            srcp/sensorsettings.ui \
            srcp/serversettings.ui \
            srcp/serverdebugconsole.ui

RESOURCES += \
    desIcons.qrc
