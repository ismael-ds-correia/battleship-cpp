#include "robotController.h"

#include "robotController.h"
#include <QDebug>

RobotController::RobotController(RobotPlayer* robot, QObject* parent)
    : QObject(parent), robot(robot) {

    soundManager = new SoundManager(this);
    soundManager->loadSounds();

}

RobotController::~RobotController() {
}

bool RobotController::attackOpponent(Player* opponent) {
    qDebug() << "RobotController::attackOpponent: Iniciando ataque.";

    if (!robot) {
        qDebug() << "RobotController::attackOpponent: RobotPlayer inválido.";
        return false;
    }

    Board& opponentBoard = opponent->getBoard();

    // Store the attacked state of all cells before the attack
    bool attackedBefore[10][10];
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            attackedBefore[i][j] = opponentBoard.getPosition(i, j).isAttacked();
        }
    }

    // Perform the attack
    robot->attack(opponentBoard);

    //soundManager->playHitSound();

    // Find the newly attacked cell
    int attackedRow = -1;
    int attackedCol = -1;
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            if (opponentBoard.getPosition(i, j).isAttacked() && !attackedBefore[i][j]) {
                attackedRow = i;
                attackedCol = j;
                break;
            }
        }
        if (attackedRow != -1) break;
    }

    if (attackedRow == -1 || attackedCol == -1) {
        qDebug() << "RobotController::attackOpponent: Não encontrou célula atacada.";
        emit attackCompleted(false);
        return false;
    }

    // Check if it was a hit
    Ship* ship = opponentBoard.getShipReference(attackedRow, attackedCol);
    bool hit = (ship != nullptr);

    // Atualiza a estratégia do robô se necessário
    if (hit && ship->isDestroyed()) {
        // Chama manualmente o ajuste para células adjacentes
        qDebug() << "Navio destruido";
        robot->adjustStrategy(opponentBoard, attackedRow, attackedCol);
        emit shipDestroyed(ship);
    }

    // Emit signals
    emit attackResult(attackedRow, attackedCol, hit);
    emit attackCompleted(hit);

    qDebug() << "Ataque do robô em (" << attackedRow << "," << attackedCol << "): " << (hit ? "Acerto" : "Erro");

    if (opponent->getFleet().isDestroyed()) {
        emit gameOver(false); // Sinaliza vitória do robô
    }


    return hit;
}


Player* RobotController::getPlayer() const {
    return static_cast<Player*>(robot);
}
