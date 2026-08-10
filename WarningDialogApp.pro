QT += core gui widgets

CONFIG += c++11

TARGET = WarningDialogApp
TEMPLATE = app

SOURCES += \
    main.cpp \
    warningdialog.cpp

HEADERS += \
    warningdialog.h

# Для Qt 5.12
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
