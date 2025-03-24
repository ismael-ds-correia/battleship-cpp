#include "GameController.h"
#include "./Headers/battleWindow.h"
#include "playerController.h"
#include "robotController.h"

GameController::GameController(
    BattleWindow* battleWindow,
    PlayerController* playerController,
    RobotController* robotController,
    QObject* parent)
    : QObject(parent),
    battleWindow(battleWindow),
    playerController(playerController),
    robotController(robotController),
    playerTurn(true){ //inicia o turno do jogador

    // Conecta o sinal de ataque do jogador ao slot correspondente
    connect(playerController, &PlayerController::attackMade, this, &GameController::onPlayerAttack);
    // Conecta o sinal de ataque da IA (inimigo) ao slot correspondente
    connect(robotController, &PlayerController::attackMade, this, &GameController::onEnemyAttack);
}

bool GameController::isPlayerTurn() const {
    return playerTurn;
}

void GameController::onPlayerAttack() {
    qDebug() << "onPlayerAttack chamado. Turno do jogador:" << playerTurn;
    // Verifica se é mesmo o turno do jogador
    if (!playerTurn) {
        qDebug() << "Ataque do jogador ignorado pois não é o seu turno.";
        return;
    }

    // Passa a vez para a IA
    nextTurn();
    qDebug() << "Turno alterado. Agora é turno da IA:" << !playerTurn;

    // Agora, se não é mais o turno do jogador, significa que é o turno da IA
    if (!playerTurn) {
        qDebug() << "Acionando ataque da IA...";
        // Chama o método que fará a IA atacar
        robotController->attackOpponent(playerController->getPlayer());
    }
}

void GameController::onEnemyAttack() {
    qDebug() << "onEnemyAttack chamado. Turno do jogador:" << playerTurn;
    // Verifica se é o turno da IA antes de processar o ataque
    if (playerTurn) {
        qDebug() << "Ataque da IA ignorado pois é o turno do jogador.";
        return;
    }
    // Lógica adicional referente ao ataque da IA (se houver)
    qDebug() << "A IA realizou seu ataque.";
    // Após o ataque da IA, alterna o turno
    nextTurn();
    qDebug() << "Turno alterado. Agora é turno do jogador:" << playerTurn;
}

void GameController::nextTurn() {
    // Alterna o turno: se era do jogador, passa para a IA, e vice-versa
    playerTurn = !playerTurn;
    // Emite sinal para notificar a mudança de turno (útil para atualizar a interface)
    emit turnChanged(playerTurn);
    qDebug() << "chamada de NextTurn(";
}
