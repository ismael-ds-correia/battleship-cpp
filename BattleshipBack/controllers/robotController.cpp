#include "robotController.h"
#include "RobotPlayer.h"
#include "Player.h"
#include "Board.h"
#include <QDebug>

RobotController::RobotController(Player* player, QObject* parent)
    : PlayerController(player, parent) {
}

RobotController::~RobotController() {
    // O player é gerenciado externamente, portanto não o deletamos aqui.
}

bool RobotController::attackOpponent(Player* opponent) {
    qDebug() << "RobotController::attackOpponent chamado.";
    RobotPlayer* robot = dynamic_cast<RobotPlayer*>(getPlayer());
    if(robot) {
        qDebug() << "RobotPlayer convertido com sucesso. Iniciando ataque.";
        Board& enemyBoard = opponent->getBoard();
        robot->attack(enemyBoard);
        qDebug() << "RobotPlayer::attack finalizado.";
        emit attackMade();
        qDebug() << "Sinal attackMade emitido.";
        return true;
    } else {
        qDebug() << "Falha ao converter Player para RobotPlayer.";
    }
    return false;
}
