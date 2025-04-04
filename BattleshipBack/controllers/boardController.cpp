#include "../controllers/boardController.h"

#include "player.h"
#include <QDebug>

BoardController::BoardController(Player* player) : owner(player) {
    // Inicialização mínima; não armazenamos board separadamente.
}

BoardController::~BoardController() {
    // Nada específico para desalocar.
}

void BoardController::placeShip(int x, int y, Ship& ship) {
    qDebug() << "BoardController: tentando posicionar navio em (" << x << "," << y << ")";
    bool success = owner->getBoard().placeShip(ship, x, y);
    if (success) {
        qDebug() << "Navio posicionado com sucesso.";
    } else {
        qDebug() << "Falha ao posicionar navio: posição inválida.";
    }
    emit boardUpdated();
}

bool BoardController::placeShipFromFleet(int shipIndex, int row, int col, bool horizontal) {
    // Delegar ao Player, que usa sua frota
    bool success = owner->placeShip(shipIndex, row, col, horizontal);
    if (success) {
        qDebug() << "Barco da frota posicionado com sucesso.";
    } else {
        qDebug() << "Falha ao posicionar barco da frota.";
    }
    emit boardUpdated();
    return success;
}

void BoardController::attackPosition(int x, int y) {
    // A lógica de ataque depende da implementação de attackOpponent do Player.
    // Aqui assumimos que x e y correspondem à linha e coluna, respectivamente.
    //bool hit = owner->attackOpponent(owner->getBoard(), x, y);
    //emit attackResult(hit);
}

void BoardController::randomizeShips() {
    if (!owner) {
        qDebug() << "BoardController: owner não encontrado.";
        return;
    }
    // Limpa o board antes de reposicionar
    clearBoard();
    owner->positionShipsRandomly();
    emit boardUpdated();
}

void BoardController::removeShip(Ship &ship) {
    // Delegamos a remoção ao método do Board, que já trata dos bloqueios e recálculos.
    owner->getBoard().removeShip(ship);
    emit boardUpdated();
}

void BoardController::clearBoard() {
    // Itera sobre todas as posições e remove os navios, além de desbloquear as posições.
    Board& board = owner->getBoard();
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            board.getPositions()[i][j].removeShip();
            board.getPositions()[i][j].unlock();
        }
    }
    emit boardUpdated();
}

Board* BoardController::getBoard() {
    return &owner->getBoard();
}

Position (&BoardController::getBoardState())[10][10] {
    return owner->getBoard().getPositions();
}
// void Controller::attackPosition(int x, int y) {
//     try {
//         board.attack(x, y);
//         bool hit = board.getPositions()[x][y].getShipReference() != nullptr;
//         emit attackResult(hit);
//         emit boardUpdated();
//     } catch (const std::exception& e) {
//         std::cerr << "erro ao atacar posição: " << e.what() << std::endl;
//     }
// }
