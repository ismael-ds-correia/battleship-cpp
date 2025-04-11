#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "board.h"
#include "player.h"

#include <QObject>
class BoardController : public QObject {
    Q_OBJECT
public:
    explicit BoardController(Player* player);
    ~BoardController();

    // Métodos de interação
    void placeShip(int x, int y, Ship& ship);
    void attackPosition(int x, int y);
    void randomizeShips();
    void removeShip(Ship &ship);
    void clearBoard();
    bool placeShipFromFleet(int shipIndex, int row, int col, bool horizontal);

    // Acesso aos dados do board
    Board* getBoard();
    Position (&getBoardState())[10][10];
    bool isPositionAvailable(int row, int col) const;

signals:
    void boardUpdated();
    void attackResult(bool hit);

private:
    Player* owner;
};


#endif
