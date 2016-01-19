#-------------------------------------------------
#
# Project created by QtCreator 2014-11-16T10:43:22
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Plants-Vs-Zombies
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    peashooter.cpp \
    sunflower.cpp \
    cherrybomb.cpp \
    wallnut.cpp \
    potatomine.cpp \
    snowpea.cpp \
    chomper.cpp \
    repeater.cpp \
    body.cpp \
    normalzombie.cpp \
    coneheadzombie.cpp \
    flagzombie.cpp \
    bucketzombie.cpp \
    newspaperzombie.cpp \
    gameobject.cpp \
    gamescene.cpp \
    sun.cpp \
    peashooterbullet.cpp \
    snowpeabullet.cpp \
    lawnmower.cpp

HEADERS  += mainwindow.h \
    peashooter.h \
    sunflower.h \
    cherrybomb.h \
    wallnut.h \
    potatomine.h \
    snowpea.h \
    chomper.h \
    repeater.h \
    body.h \
    normalzombie.h \
    coneheadzombie.h \
    flagzombie.h \
    bucketzombie.h \
    newspaperzombie.h \
    gameobject.h \
    gamescene.h \
    sun.h \
    peashooterbullet.h \
    snowpeabullet.h \
    lawnmower.h

FORMS    += mainwindow.ui

OTHER_FILES += \
    pvz_levels.csv \
    pvz_players.csv

RESOURCES += \
    resources.qrc
