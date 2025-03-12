#include "playerController.h"
#include "Player.h"
#include <QDebug>

PlayerController::PlayerController(Player* player, QObject* parent)
    : QObject(parent), player(player) {
}

PlayerController::~PlayerController() {
    // O Player é gerenciado por outra parte, então não o deletamos aqui.
}

QString PlayerController::getPlayerName() const {
    return QString::fromStdString(player->getName());
}
/*
void PlayerController::placeFleet() {
    player->placeFleet();
    emit playerUpdated();
}
*/
bool PlayerController::placeShipFromFleet(int shipIndex, int row, int col, bool horizontal) {
    // Chama o método do Player que posiciona o barco da frota no tabuleiro.
    bool success = player->placeShip(shipIndex, row, col, horizontal);
    if (success) {
        emit playerUpdated();
    }
    return success;
}

bool PlayerController::repositionShip(int shipIndex, int newLine, int newColumn, bool newHorizontal) {
    bool success = player->repositionShip(shipIndex, newLine, newColumn, newHorizontal);
    emit playerUpdated();
    return success;
}

void PlayerController::positionShipsRandomly() {
    //reinicia o estado do board antes de posicionar os barcos
    resetBoard();

    // Posiciona os barcos aleatoriamente no objeto player
    player->positionShipsRandomly();

    emit playerUpdated();
}


void PlayerController::resetBoard() {
    Board& board = player->getBoard();

    for (int i = 0; i< 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            board.getPositions()[i][j].removeShip();
            board.getPositions()[i][j].unlock();
        }
    }
    emit playerUpdated();
}

bool PlayerController::attackOpponent(Player* opponent, int row, int col) {
    bool hit = player->attackOpponent(opponent->getBoard(), row, col);
    emit attackResult(hit);
    return hit;
}

void PlayerController::printBoard() const {
    player->printBoard();
}
