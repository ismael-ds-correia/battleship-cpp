#ifndef ROBOTCONTROLLER_H
#define ROBOTCONTROLLER_H

#include "playerController.h"
#include "robotplayer.h"
#include "../BattleshipFront/Headers/soundManager.h"

class RobotController : public QObject {
    Q_OBJECT
public:
    explicit RobotController(RobotPlayer* robot, QObject* parent = nullptr);
    ~RobotController();

    bool attackOpponent(Player* opponent);
    Player* getPlayer() const;

signals:
    // Sinal de ataque concluído; como a IA não precisa de coordenadas, passamos apenas o status
    void attackCompleted(bool hit);
    void attackResult(int row, int col, bool hit);
    void gameOver(bool playerWon);
    void shipDestroyed(Ship* ship);

private:
    RobotPlayer* robot;

    SoundManager* soundManager;
};

#endif // ROBOTCONTROLLER_H
