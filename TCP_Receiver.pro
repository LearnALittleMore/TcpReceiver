QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TCP_Receiver
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
        main.cpp \
        msgrecv/msgrecver.cpp \
        widget/widget.cpp \
        server/server.cpp \
        server/epoller.cpp \

HEADERS += \
        msgrecv/msgrecver.h \
        widget/widget.h \
        server/server.h \
        server/epoller.h \

INCLUDEPATH += \
        ./widget \
        ./server \
        ./msgrecv \

RC_FILE += ./resource/logo.rc

RESOURCES += res.qrc
