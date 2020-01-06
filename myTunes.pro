#-------------------------------------------------
#
# Project created by QtCreator 2015-12-03T21:59:25
#
#-------------------------------------------------

QT       += core gui multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = myTunes
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    playbar.cpp \
    musicstatus.cpp \
    musiclibrary.cpp

HEADERS  += mainwindow.h \
    playbar.h \
    musicstatus.h \
    musiclibrary.h

FORMS    += mainwindow.ui \
    playbar.ui \
    musicstatus.ui

RESOURCES +=
