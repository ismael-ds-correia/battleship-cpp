#include "controller.h"

Controller::Controller() : board() {}

Controller::~Controller() {}

void Controller::placeShip(int x, int y, Ship& ship) {
    try {
        board.placeShip(ship, x, y);
        emit boardUpdated();
    } catch (const std::exception& e) {
        std::cerr << "erro ao posicionar navio: " << e.what() <<std::endl;
    }
}

Position (&Controller::getBoardState())[10][10] {
    return board.getPositions();
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

