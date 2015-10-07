QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = nunc
TEMPLATE = app

#openssl
unix:LIBS += -lssl -lcrypto

SOURCES += main.cpp\
        mainwindow.cpp \
    diary.cpp \
    entry.cpp

HEADERS  += mainwindow.h \
    diary.h \
    entry.h

FORMS    += mainwindow.ui
