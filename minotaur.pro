#-------------------------------------------------
#
# Project created by QtCreator 2016-01-23T17:11:11
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = minotaur
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    controller.cpp \
    actuator.cpp \
    logger.cpp \
    clock_time.cpp

HEADERS  += mainwindow.h \
    controller.h \
    actuator.h \
    logger.h \
    clock_time.h

FORMS    += mainwindow.ui
