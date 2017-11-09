#-------------------------------------------------
#
# Project created by QtCreator 2016-01-23T17:11:11
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_VERSION, 5.7): QT += widgets

# Use C++11 compiler
QMAKE_CXXFLAGS += -std=c++11

TARGET = minotaur
TEMPLATE = app
DESTDIR = builds

# Include third party libraries
include (third-party/qextserialport.pri)


SOURCES += \
    code/main.cpp \
    code/controller/actuator.cpp \
    code/controller/controller.cpp \
    code/gui/mainwindow.cpp \
    code/utility/clock_time.cpp \
    code/utility/logger.cpp \
    code/gui/actuatorsetup.cpp \
    code/controller/simulator.cpp \
    code/gui/simulatorwindow.cpp \
    code/utility/vector2i.cpp \
    code/graphics/simulatorscene.cpp \
    code/graphics/robotgraphicsitem.cpp \
    code/gui/pythoninterpreter.cpp \
    code/gui/actionabout.cpp \
    code/gui/scriptwindow.cpp \

HEADERS  += \
    code/controller/actuator.h \
    code/controller/controller.h \
    code/gui/mainwindow.h \
    code/utility/clock_time.h \
    code/utility/logger.h \
    code/gui/actuatorsetup.h \
    code/controller/simulator.h \
    code/gui/simulatorwindow.h \
    code/utility/vector2i.h \
    code/graphics/simulatorscene.h \
    code/graphics/robotgraphicsitem.h \
    code/gui/pythoninterpreter.h \
    code/gui/actionabout.h \
    code/gui/scriptwindow.h \

FORMS += code/gui/mainwindow.ui \
    code/gui/actuatorsetup.ui \
    code/gui/simulatorwindow.ui \
    code/gui/pythoninterpreter.ui \
    code/gui/actionabout.ui \
    code/gui/scriptwindow.ui \
