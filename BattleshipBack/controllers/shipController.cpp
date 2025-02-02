#include "shipController.h"

#include "position.h"

ShipController::ShipController() : ship() {}

ShipController::~ShipController() {}

//busca a posição inicial do barco diretamente a partir do shipReference
std::pair<int, int> ShipController::getShipStartPosition(Position boardState[10][10], Ship* ship) {
    if (!ship) return {-1, -1}; //proteção contra ponteiro nulo

    //percorre a matriz até achar uma celula com um barco
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            if (boardState[i][j].getShipReference() == ship) {
                int row = i, col = j;

                //se for horizontal, retorna as colunas se mantendo na mesma linha até encontrar a posição inicial
                if (ship->isHorizontal()) {
                    while (col > 0 && boardState[row][col - 1].getShipReference() == ship) {
                        --col;
                    }
                }
                // Se for vertical, retorna as linhas se mantendo na mesma coluna até enontrar a posição inicial
                else {
                    while (row > 0 && boardState[row - 1][col].getShipReference() == ship) {
                        --row;
                    }
                }

                // Agora row e col apontam para a posição inicial do navio
                return {row, col};
            }
        }
    }

    return {-1, -1}; // Retorna nullptr caso o barco não seja encontrado
}



//busca a posição inicial do barco passado como argumento
// Position* ShipController::getShipStartPosition(Board& board, Ship* ship) {
//     if (!ship) return nullptr;//evita o uso em objetos que não são barcos

//     Position (&positions)[10][10] = board.getPositions();

//     for (int i = 0; i < 10; ++i) {
//         for (int j = 0; j < 10; ++j) {
//             if (positions[i][j].getShipReference() == ship) {
//                 if (ship->isHorizontal()) {//se for horizontal
//                     if (j == 0 || positions[i][j - 1].getShipReference() != ship) {//se o elemento anterior a posição atual
//                         return &positions[i][j];                                   //for agua ou 0, indica que esta é a posição inicial
//                     }
//                 } else {// se for vertical
//                     if (i == 0 || positions[i - 1][j].getShipReference() != ship) {
//                         return &positions[i][j];
//                     }
//                 }
//             }
//         }
//     }

//     return nullptr;
// }

// int ShipController::getShipPartIndex(Board& board, Ship* ship, Position* pos) {
//     if (!ship || !pos) {
//         return -1;
//     }

//     Position* startPos = getShipStartPosition(board,ship);

//     if (!startPos) {
//         return -1;
//     }

//     int index = 0;

//     for (int i = 0; i < ship->getSize(); i++) {
//         Position* currentPos;

//         if (ship->isHorizontal()) {
//             currentPos = &board[startPos->][];
//         }
//     }

// }


// int ShipController::getShipPartIndex(Ship* ship, int row, int col) {
//     Position startPos = ship->getStartPosition(); //precisa implementar

//     if (!ship) {
//         return -1;

//     } else if (ship->isHorizontal()) {
//         return col - startPos.col;

//     } else {
//         return row - startPos.row;
//     }



