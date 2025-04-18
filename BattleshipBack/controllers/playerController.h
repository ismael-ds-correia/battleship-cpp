#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

#include "ship.h"
#include <string>
#include <QObject>
#include <QString>

class Player;

class PlayerController : public QObject {
    Q_OBJECT
public:
    explicit PlayerController(Player* player, QObject* parent = nullptr);
    ~PlayerController();

    QString getPlayerName() const;
    void placeFleet();
    bool placeShipFromFleet(int shipIndex, int row, int col, bool horizontal);
    bool repositionShip(int shipIndex, int newLine, int newColumn, bool newHorizontal);
    void positionShipsRandomly();
    void resetBoard();
    virtual bool attackOpponent(Player* opponent, int row, int col);
    void printBoard() const;
    Player* getPlayer() const;

signals:
    void playerUpdated();
    void attackResult(int row, int col, bool hit);
    void shipDestroyed(Ship* ship);

private:
    Player* player;
};

#endif // PLAYERCONTROLLER_H
