cmake_minimum_required(VERSION 3.30)
project(battleship_cpp)

set(CMAKE_CXX_STANDARD 14)

include_directories(BattleshipBack/controllers)
include_directories(BattleshipBack/include)
include_directories(BattleshipFront/Headers)

add_executable(battleship_cpp
    BattleshipBack/controllers/boardController.cpp
    BattleshipBack/controllers/boardController.h
    BattleshipBack/controllers/shipController.cpp
    BattleshipBack/controllers/shipController.h
    BattleshipBack/include/attack.h
    BattleshipBack/include/board.h
    BattleshipBack/include/CircularLinkedList.h
    BattleshipBack/include/fleet.h
    BattleshipBack/include/player.h
    BattleshipBack/include/position.h
    BattleshipBack/include/robotplayer.h
    BattleshipBack/include/ship.h
    BattleshipBack/src/attack.cpp
    BattleshipBack/src/board.cpp
    BattleshipBack/src/CircularLinkedList.cpp
    BattleshipBack/src/fleet.cpp
    BattleshipBack/src/main.cpp
    BattleshipBack/src/player.cpp
    BattleshipBack/src/position.cpp
    BattleshipBack/src/robotplayer.cpp
    BattleshipBack/src/ship.cpp)
