QT += qml quick
CONFIG += c++11

TARGET = nunc
TEMPLATE = app

#openssl
unix:LIBS += -lssl -lcrypto

SOURCES += \
       main.cpp\
       diary.cpp \
       entry.cpp \
       diarymodel.cpp

HEADERS += \
       diary.h \
       entry.h \
       diarymodel.h

RESOURCES += qml.qrc
