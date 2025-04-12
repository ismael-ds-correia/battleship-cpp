#include <iostream>
#include "board.h"

using namespace std;

// Método para posicionar um navio no tabuleiro.
bool Board::placeShip(Ship& ship, int line, int column) {
    if(!checkPosition(ship, line, column)){
        return false;
    }
    // Posiciona o navio na matriz de posições.
    if(ship.isHorizontal()){
        for(int i = column; i < column + ship.getSize(); i++){
            this->positions[line][i].placeShip(ship);
        }
    }else{
        for(int i = line; i < line + ship.getSize(); i++){
            this->positions[i][column].placeShip(ship);
        }
    }
    // Bloqueia as posições adjacentes ao navio.
    blockPositions(ship, line, column);

    return true;
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
    int startRow = max(0, line - 1); // Linha inicial (garantindo que não seja menor que 0).
    int endRow = min(9, line + (ship.isHorizontal() ? 0 : ship.getSize() - 1) + 1); // Linha final.

    // Definindo os limites das colunas a serem bloqueadas.
    int startCol = max(0, column - 1); // Coluna inicial (garantindo que não seja menor que 0).
    int endCol = min(9, column + (ship.isHorizontal() ? ship.getSize() - 1 : 0) + 1); // Coluna final.

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

// Método sobrecarregado para verificar se é possível atacar uma posição.
bool Board::checkPosition(int row, int column){
    if(row < 0 || row > 9 || column < 0 || column > 9){
        return false;
    }

    return !(this->positions[row][column].isAttacked());
}

// Método para imprimir o estado atual do tabuleiro (usado para testes).
void Board::print() {
    for (int i = 0; i < 10; i++) { // Itera pelas linhas.
        for (int j = 0; j < 10; j++) { // Itera pelas colunas.
            if (this->positions[i][j].isAttacked()) { // Posição atacada.
                if(this->positions[i][j].getShipReference() != nullptr){
                    cout << "- ";
                    continue;
                }

                cout << "x "; // Marca como atacada.
                continue;
            } else if (this->positions[i][j].getShipReference() != nullptr) { // Posição bloqueada.
                cout << "B "; // Marca como bloqueada.
                continue;
            }

            // Posição válida e não atacada.
            cout << "O "; // Marca como livre.
        }
        cout << "\n"; // Nova linha após cada linha do tabuleiro.
    }
}

bool Board::attack(int row, int column){
    cout << "atacando (" << row << ", " << column << ")\n";
    if(this->checkPosition(row, column)){
        this->positions[row][column].attack();

        return true;
    }

    return false;
}

bool Board::attackFront(int row, int col) {
    cout << "atacando (" << row << ", " << col << ")\n";
    if (this->checkPosition(row, col)) {
        //marca a posição como atacada.
        this->positions[row][col].attack();

        Ship* ship = this->positions[row][col].getShipReference();
        if (ship != nullptr) {
            return true;  //acertou um navio.
        }
        return false; //acertou a água.
    }
    return false;
}

void Board::markAdjacentAsAttacked(Ship& ship, int line, int column) {
    int startRow = max(0, line - 1);
    int endRow = min(9, line + (ship.isHorizontal() ? 0 : ship.getSize() - 1) + 1);
    int startCol = max(0, column - 1);
    int endCol = min(9, column + (ship.isHorizontal() ? ship.getSize() - 1 : 0) + 1);

    for (int i = startRow; i <= endRow; i++) {
        for (int j = startCol; j <= endCol; j++) {
            // Se a célula ainda não foi atacada, a marca
            if (!this->positions[i][j].isAttacked()) {
                this->positions[i][j].attack();
            }
        }
    }
}

// Método para retornar uma referência à matriz de posições.
Position (&Board::getPositions())[10][10] {
    return this->positions;
}

// Retorna uma posição com base em um par de valores.
Position Board::getPosition(int row, int column){
    return this->positions[row][column];
}

Ship* Board::getShipReference(int row, int column) {
    return this->positions[row][column].getShipReference();
}

bool Board::isAttacked(int row, int column){
    return this->positions[row][column].isAttacked();
}
