#-------------------------------------------------
#
# Project created by QtCreator 2016-01-21T18:12:56
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BookmarkManager
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    bookmark.cpp \
    hotkey.cpp

HEADERS  += mainwindow.h \
    bookmark.h \
    hotkey.h

FORMS    += mainwindow.ui

win32:LIBS += -lWS2_32 -luser32
