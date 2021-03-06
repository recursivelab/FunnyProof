#-------------------------------------------------
#
# Project created by QtCreator 2016-03-23T10:36:07
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FunnyProof
TEMPLATE = app
CONFIG += c++11

SOURCES += \
    mainwindow.cpp \
    dictionary.cpp \
    language.cpp \
    main.cpp \
    readwrite.cpp \
    utility.cpp \
    theory.cpp

HEADERS  += \
    config.h \
    dictionary.h \
    dictionary_imp.h \
    error.h \
    language.h \
    language_imp.h \
    mainwindow.h \
    readwrite.h \
    utility.h \
    utility_imp.h \
    theory.h

FORMS    += mainwindow.ui
