QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
INCLUDEPATH += ../BattleshipBack/include \
    ../BattleshipBack/src \
    ../BattleshipBack/controllers

SOURCES += \
    ../BattleshipBack/controllers/boardController.cpp \
    ../BattleshipBack/src/board.cpp \
    ../BattleshipBack/src/fleet.cpp \
    ../BattleshipBack/src/player.cpp \
    ../BattleshipBack/src/position.cpp \
    ../BattleshipBack/src/ship.cpp \
    Sources/mainwindow.cpp \
    main.cpp \

HEADERS += \
    ../BattleshipBack/controllers/boardController.h \
    ../BattleshipBack/include/board.h \
    ../BattleshipBack/include/fleet.h \
    ../BattleshipBack/include/player.h \
    ../BattleshipBack/include/position.h \
    ../BattleshipBack/include/ship.h \
    Headers/mainwindow.h \

FORMS += \
    Forms/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    textures.qrc
