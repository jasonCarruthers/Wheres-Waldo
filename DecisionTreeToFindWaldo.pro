#-------------------------------------------------
#
# Project created by QtCreator 2016-11-15T13:33:57
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DecisionTreeToFindWaldo
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    myimage.cpp \
    drawwindow.cpp \
    feature.cpp \
    vector2i.cpp \
    region.cpp \
    decisiontree.cpp

HEADERS  += mainwindow.h \
    myimage.h \
    drawwindow.h \
    feature.h \
    vector2i.h \
    region.h \
    decisiontree.h

FORMS    += mainwindow.ui
