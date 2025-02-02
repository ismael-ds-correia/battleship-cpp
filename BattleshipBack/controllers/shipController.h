#ifndef SHIPCONTROLLER_H
#define SHIPCONTROLLER_H

#include "ship.h"
#include "position.h"
#include "board.h"
#include <QObject>

class ShipController : public QObject {

private:
    Ship ship;
    Board* board;

public:
    ShipController();
    ShipController(Board* board) : board(board) {}
    ~ShipController();
    std::pair<int, int> getShipStartPosition(Position boardState[10][10], Ship* ship);
    int getShipPartIndex(Board& board, Ship* ship, Position* pos);

};

#endif // SHIPCONTROLLER_H
