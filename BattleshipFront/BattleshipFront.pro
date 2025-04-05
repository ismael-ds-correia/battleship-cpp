QT       += core gui
DEFINES += USE_QT

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
    ../BattleshipBack/controllers/playerController.cpp \
    ../BattleshipBack/controllers/robotController.cpp \
    ../BattleshipBack/controllers/shipController.cpp \
    ../BattleshipBack/src/board.cpp \
    ../BattleshipBack/src/fleet.cpp \
    ../BattleshipBack/src/player.cpp \
    ../BattleshipBack/src/robotplayer.cpp \
    ../BattleshipBack/src/position.cpp \
    ../BattleshipBack/src/ship.cpp \
    Sources/battleWindow.cpp \
    Sources/selectorSpace.cpp \
    Sources/boardCell.cpp \
    Sources/boardRenderer.cpp \
    Sources/mainwindow.cpp \
    main.cpp \

HEADERS += \
    ../BattleshipBack/controllers/boardController.h \
    ../BattleshipBack/controllers/playerController.h \
    ../BattleshipBack/controllers/robotController.h \
    ../BattleshipBack/controllers/shipController.h \
    ../BattleshipBack/include/board.h \
    ../BattleshipBack/include/fleet.h \
    ../BattleshipBack/include/player.h \
    ../BattleshipBack/include/robotplayer.h \
    ../BattleshipBack/include/position.h \
    ../BattleshipBack/include/ship.h \
    Headers/battleWindow.h \
    Headers/boardRenderer.h \
    Headers/boardCell.h \
    Headers/mainwindow.h \
    Headers/selectorSpace.h

FORMS += \
    Forms/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    textures.qrc
