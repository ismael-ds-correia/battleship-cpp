#include "robotController.h"

#include "robotController.h"
#include <QDebug>

RobotController::RobotController(RobotPlayer* robot, QObject* parent)
    : QObject(parent), robot(robot)
{
    // robot deve ter sido criado e inicializado fora
}

RobotController::~RobotController() {
    // Caso o gerenciamento de memória seja feito fora, não delete aqui
    // delete robot;  // se for de responsabilidade do RobotController
}

bool RobotController::attackOpponent(Player* opponent) {
    qDebug() << "RobotController::attackOpponent: Iniciando ataque.";

    if (!robot) {
        qDebug() << "RobotController::attackOpponent: RobotPlayer inválido.";
        return false;
    }

    qDebug() << "RobotController::attackOpponent: Chamando robot->attack.";
    robot->attack(opponent->getBoard());
    qDebug() << "RobotController::attackOpponent: Retornando de robot->attack.";

    // Para efeitos de depuração, consideramos que o ataque sempre “acertou”
    bool hit = true;

    qDebug() << "RobotController::attackOpponent: Emissão de sinal attackCompleted com hit =" << hit;
    emit attackCompleted(hit);

    return hit;
}


Player* RobotController::getPlayer() const {
    return static_cast<Player*>(robot);
}
