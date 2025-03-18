#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

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
    bool attackOpponent(Player* opponent, int row, int col);
    void printBoard() const;

signals:
    void playerUpdated();
    void attackResult(int row, int col, bool hit);

private:
    Player* player;
};

#endif // PLAYERCONTROLLER_H
