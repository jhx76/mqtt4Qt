#-------------------------------------------------
#
# Project created by QtCreator 2016-03-21T23:22:51
#
#-------------------------------------------------
#This is a new comment

QT       -= gui

QT += network

TARGET = mqtt4Qt
TEMPLATE = lib

DEFINES += MQTT4QT_LIBRARY

# Adapt this path to your platform
LIBS += \
    -L/usr/lib/ -lmosquittopp

# This is a comment in the .pro file
SOURCES += \
    QMqttMessage.cpp \
    QMqttClient.cpp

HEADERS += \
    mqtt4qt_global.h \
    QMqttMessage.h \
    QMqttClient.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
