#include <iostream>
#include "board.h"

using namespace std;

// Método para posicionar um navio no tabuleiro.
void Board::placeShip(Ship& ship, int line, int column) {
    // Posiciona o navio na matriz de posições.
    this->positions[line][column].placeShip(ship);
    // Bloqueia as posições adjacentes ao navio.
    blockPositions(ship, line, column);
}

// Método para remover um navio que já foi posicionado.
void Board::removeShip(Ship& ship){
    // Percorre todas as posições do tabuleiro.
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 10; j++){
            // Obtém a referência ao navio na posição atual.
            Ship* currentShip = this->positions[i][j].getShipReference();

            // Verifica se a posição pertence ao navio a ser removido.
            if(currentShip == &ship){
                // Remove a referência ao navio na posição.
                this->positions[i][j].removeShip();
                // Desbloqueia a posição.
                this->positions[i][j].unlock();
            }
        }
    }

    // Recalcula bloqueios para outros navios ainda no tabuleiro.
    // Isso é necessário caso o desbloqueio anterior tenha afetado os bloqueios de outros navios.
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 10; j++){
            Ship* currentShip = this->positions[i][j].getShipReference();
            if(currentShip != nullptr){
                blockPositions(*currentShip, i, j);
            }
        }
    }
}

// Método para bloquear posições adjacentes ao navio.
void Board::blockPositions(Ship& ship, int line, int column) {
    // Definindo os limites das linhas a serem bloqueadas.
    int startRow = std::max(0, line - 1); // Linha inicial (garantindo que não seja menor que 0).
    int endRow = std::min(9, line + (ship.isHorizontal() ? 0 : ship.getSize() - 1) + 1); // Linha final.

    // Definindo os limites das colunas a serem bloqueadas.
    int startCol = std::max(0, column - 1); // Coluna inicial (garantindo que não seja menor que 0).
    int endCol = std::min(9, column + (ship.isHorizontal() ? ship.getSize() - 1 : 0) + 1); // Coluna final.

    // Iterando pelas posições adjacentes ao navio.
    for (int i = startRow; i <= endRow; i++) {
        for (int j = startCol; j <= endCol; j++) {
            // Bloqueia a posição atual.
            this->positions[i][j].block();
        }
    }
}

// Método para verificar se um navio pode ser colocado em uma posição específica.
bool Board::checkPosition(Ship& ship, int line, int column) {
    if (ship.isHorizontal()) { // Se o navio está na horizontal.
        // Verifica se o navio ultrapassará os limites do tabuleiro.
        if (ship.getSize() + column > 10) {
            return false;
        }

        // Verifica se as posições estão disponíveis.
        for (int i = column; i < column + ship.getSize(); i++) {
            if (!this->positions[line][i].isValidPosition()) {
                return false;
            }
        }
    } else { // Se o navio está na vertical.
        // Verifica se o navio ultrapassará os limites do tabuleiro.
        if (ship.getSize() + line > 10) {
            return false;
        }

        // Verifica se as posições estão disponíveis.
        for (int i = line; i < ship.getSize() + line; i++) {
            if (!this->positions[i][column].isValidPosition()) {
                return false;
            }
        }
    }

    // Retorna verdadeiro se todas as verificações forem aprovadas.
    return true;
}

// Método para imprimir o estado atual do tabuleiro (usado para testes).
void Board::print() {
    for (int i = 0; i < 10; i++) { // Itera pelas linhas.
        for (int j = 0; j < 10; j++) { // Itera pelas colunas.
            if (this->positions[i][j].isAttacked()) { // Posição atacada.
                cout << "x "; // Marca como atacada.
                continue;
            } else if (this->positions[i][j].isBlocked()) { // Posição bloqueada.
                cout << "b "; // Marca como bloqueada.
                continue;
            }

            // Posição válida e não atacada.
            cout << "o "; // Marca como livre.
        }
        cout << "\n"; // Nova linha após cada linha do tabuleiro.
    }
}

// Método para retornar uma referência à matriz de posições.
Position (&Board::getPositions())[10][10] {
    return this->positions;
}
