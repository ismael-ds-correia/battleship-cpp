#include <iostream>
#include <Player.h>
#include "robotplayer.h"
#include <queue>
#include <vector>
#include <utility>
#include "board.h"
#include <random>
#include <ship.h>
#include <cstdlib>
#include <ctime>
#include <QTimer>

using namespace std;

RobotPlayer::RobotPlayer() : Player("Xerath"){
	for(int i = 0; i<10; i++){
		for(int j = 0; j<10; j++){
			this->virtualBoard[i][j] = 0;
		}
	}
}

void RobotPlayer::attack(Board& enemyBoard) {
    if (!priorityQueue.empty()) {
        auto target = priorityQueue.front();
        priorityQueue.pop();
        int row = target.first;
        int column = target.second;
        enemyBoard.attack(row, column);
        adjustStrategy(enemyBoard, row, column);
    } else {
        if (shouldAttackStrategicPositions()) {
            cout << "Modo de ataque estrategico ativado!" << "\n";
            planStrategicAttack(enemyBoard);
            if (!priorityQueue.empty()) {
                auto target = priorityQueue.front();
                priorityQueue.pop();
                int row = target.first;
                int column = target.second;
                enemyBoard.attack(row, column);
                adjustStrategy(enemyBoard, row, column);
            } else {
                // Caso a estratégia não encontre um alvo, ataque aleatoriamente.
                int row = rand() % 10;
                int column = rand() % 10;
                while (!enemyBoard.attack(row, column)) {
                    row = rand() % 10;
                    column = rand() % 10;
                }
                adjustStrategy(enemyBoard, row, column);
            }
        } else {
            // Ataque aleatório
            int row = rand() % 10;
            int column = rand() % 10;
            while (!enemyBoard.attack(row, column)) {
                row = rand() % 10;
                column = rand() % 10;
            }
            adjustStrategy(enemyBoard, row, column);
        }
    }
}

void RobotPlayer::adjustStrategy(Board& enemyBoard, int row, int column) {
    Ship* ship = enemyBoard.getShipReference(row, column);
    if (ship != nullptr) {
        if (ship->isDestroyed()) {
            // Atualiza o virtualBoard para indicar que a célula foi “fechada”
            this->virtualBoard[row][column] = 3;
            // Realiza os ajustes de estratégia e limpa a fila de prioridade
            wreckedShipAdjustment(enemyBoard, row, column);
            clearProrityQueue();
            oneLessShip(ship->getSize());

            // Calcula a posição inicial do navio (para saber quais células adjacentes marcar)
            pair<int, int> startPos = locateShipStart(enemyBoard, row, column);

            //adia essa chamada para dar tempo para a atualização do estado e dos sinais do frontend serem processados.
            //------------------------------ UTILIZAÇÃO DE THREADS ------------------------------
            thread([=, &enemyBoard]() {
                this_thread::sleep_for(chrono::milliseconds(10));
                enemyBoard.markAdjacentAsAttacked(*ship, startPos.first, startPos.second);
            }).detach();
            //----------------------------------------------------------------------------------
        } else {
            this->virtualBoard[row][column] = 2;
            discoverDirectionAndAdd(enemyBoard, row, column);
        }
    } else {
        this->virtualBoard[row][column] = 1;
    }
}

void RobotPlayer::discoverDirectionAndAdd(Board& enemyBoard, int row, int column){
    // Determinar se o navio está alinhado horizontalmente ou verticalmente
    bool horizontal = false;
    bool vertical = false;

    if(isValidForTesting(row, column - 1) && virtualBoard[row][column - 1] == 2) horizontal = true;
    if(isValidForTesting(row, column + 1) && virtualBoard[row][column + 1] == 2) horizontal = true;
    if(isValidForTesting(row - 1, column) && virtualBoard[row - 1][column] == 2) vertical = true;
    if(isValidForTesting(row + 1, column) && virtualBoard[row + 1][column] == 2) vertical = true;

    this->clearProrityQueue();
    // Baseado na direção, adicionar à fila de prioridades
    if(horizontal){
        int c = column;
        while(this->virtualBoard[row][c-1] != 0){
            if(this->virtualBoard[row][c-1] == 3 || this->virtualBoard[row][c-1] == 1){
                break;
            }

            c--;
        }

        this->addToPriorityQueue(row, c-1);

        c = column;
        while(this->virtualBoard[row][c+1] != 0){
            if(this->virtualBoard[row][c+1] == 3 || this->virtualBoard[row][c+1] == 1){
                break;
            }

            c++;
        }

        this->addToPriorityQueue(row, c+1);

        return;
    }

    if(vertical){
        int r = row;
        while(this->virtualBoard[r-1][column] != 0){
            if(this->virtualBoard[r-1][column] == 3 || this->virtualBoard[r-1][column] == 1){
                break;
            }

            r--;
        }

        this->addToPriorityQueue(r-1, column);

        r = row;
        while(this->virtualBoard[r+1][column] != 0){
            if(this->virtualBoard[r+1][column] == 3 || this->virtualBoard[r+1][column] == 1){
                break;
            }

            r++;
        }

        this->addToPriorityQueue(r+1, column);

        return;
    }

    attackNeighbors(row, column);
}

void RobotPlayer::wreckedShipAdjustment(Board& enemyBoard, int row, int column){
    Ship* ship = enemyBoard.getShipReference(row, column);

    // Determinar os limites do navio (horizontal ou vertical)
    int startRow = row, endRow = row;
    int startCol = column, endCol = column;

    // Expandir para cima e para baixo (se vertical)
    while(startRow > 0 && enemyBoard.getShipReference(startRow - 1, column) == ship){
        startRow--;
    }
    while(endRow < 9 && enemyBoard.getShipReference(endRow + 1, column) == ship){
        endRow++;
    }

    // Expandir para esquerda e direita (se horizontal)
    while(startCol > 0 && enemyBoard.getShipReference(row, startCol - 1) == ship){
        startCol--;
    }
    while(endCol < 9 && enemyBoard.getShipReference(row, endCol + 1) == ship){
        endCol++;
    }

    // Determinar os limites adjacentes (1 célula ao redor do navio)
    int adjStartRow = max(0, startRow - 1);
    int adjEndRow = min(9, endRow + 1);
    int adjStartCol = max(0, startCol - 1);
    int adjEndCol = min(9, endCol + 1);

    // Marcar as posições adjacentes no virtualBoard
    for(int i = adjStartRow; i <= adjEndRow; i++){
        for(int j = adjStartCol; j <= adjEndCol; j++){
            this->virtualBoard[i][j] = 3; // Marca como posição bloqueada
        }
    }
}

bool RobotPlayer::shouldAttackStrategicPositions(){
    int emptyPositions = 0;
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 10; j++){
            if(this->virtualBoard[i][j] != 0){
                emptyPositions++;
            }
        }
    }

    return(emptyPositions/100.0 > 0.2);
}

void RobotPlayer::planStrategicAttack(Board& enemyBoard) {
    int size = sizeOfTheNextShip(); // Tamanho do próximo navio que se espera atacar
    if (size == 0) return;

    // Escolhe aleatoriamente uma direção para começar a busca
    bool tryVertical = (rand() % 2 == 0);
    bool found = false;

    if (tryVertical) {
        found = searchVertically(size);
        if (!found)
            found = searchHorizontally(size);
    } else {
        found = searchHorizontally(size);
        if (!found)
            found = searchVertically(size);
    }

    //Se found ainda for false, a fila de prioridades ficará vazia e um ataque aleatório será usado.
}

//Função auxiliar que retorna o ponto de início do navio.
//(a célula mais à esquerda se horizontal ou a mais acima se vertical).
std::pair<int, int> RobotPlayer::locateShipStart(Board& enemyBoard, int row, int column) {
    Ship* ship = enemyBoard.getShipReference(row, column);
    if (ship == nullptr) {
        //Se não houver navio, retorna a posição original.
        return {row, column};
    }
    
    int startRow = row;
    int startCol = column;
    
    if (ship->isHorizontal()) {
        //Procura o extremo esquerdo.
        while (startCol > 0 && enemyBoard.getShipReference(row, startCol - 1) == ship) {
            startCol--;
        }
    } else {
        //Procura o extremo superior.
        while (startRow > 0 && enemyBoard.getShipReference(startRow - 1, column) == ship) {
            startRow--;
        }
    }
    
    return {startRow, startCol};
}

bool RobotPlayer::searchVertically(int size) {
    //Percorre cada coluna do tabuleiro virtual.
    for (int j = 0; j < 10; j++) {
        int contiguous = 0;
        for (int i = 0; i < 10; i++) {
            if (this->virtualBoard[i][j] == 0) {
                contiguous++;
            } else {
                contiguous = 0;
            }
            if (contiguous >= size) {
                //Calcula a linha alvo como o centro da sequência.
                int targetRow = i - size / 2;
                if (targetRow < 0) targetRow = 0;
                if (targetRow > 9) targetRow = 9;
                addToPriorityQueue(targetRow, j);
                return true;
            }
        }
    }
    return false;
}

bool RobotPlayer::searchHorizontally(int size) {
    //Percorre cada linha do tabuleiro virtual.
    for (int i = 0; i < 10; i++) {
        int contiguous = 0;
        for (int j = 0; j < 10; j++) {
            if (this->virtualBoard[i][j] == 0) {
                contiguous++;
            } else {
                contiguous = 0;
            }
            if (contiguous >= size) {
                //Calcula a coluna alvo como o centro da sequência.
                int targetColumn = j - size / 2;
                //Se necessário, garante que o target esteja dentro dos limites.
                if (targetColumn < 0) targetColumn = 0;
                if (targetColumn > 9) targetColumn = 9;
                addToPriorityQueue(i, targetColumn);
                return true;
            }
        }
    }
    return false;
}


int RobotPlayer::sizeOfTheNextShip(){
    for(int i = 0; i < 4; i++){
        if(this->sizeOfEnemyShips[i] != 0){
            return this->sizeOfEnemyShips[i];
        }
    }

    return 0;
}

void RobotPlayer::oneLessShip(int sizeOfShipDestroyed){
    for(int i = 0; i < 4; i++){
        if(this->sizeOfEnemyShips[i] == sizeOfShipDestroyed){
            this->sizeOfEnemyShips[i] = 0;
            return;
        }
    }
}

void RobotPlayer::attackNeighbors(int row, int column){
	this->clearProrityQueue();
	addToPriorityQueue(row - 1, column);
    addToPriorityQueue(row + 1, column);
    addToPriorityQueue(row, column - 1);
    addToPriorityQueue(row, column + 1);
}

void RobotPlayer::addToPriorityQueue(int row, int column){
	if(this->isValid(row, column)){
		this->priorityQueue.push({row, column});
	}
}

void RobotPlayer::clearProrityQueue(){
	while(!this->priorityQueue.empty()){
		this->priorityQueue.pop();
	}
}

bool RobotPlayer::isValid(int row, int column){
	if(row < 0 || row > 9 || column < 0 || column > 9 || this->virtualBoard[row][column] != 0){
		return false;
	}

	return true;
}

bool RobotPlayer::isValidForTesting(int row, int column){
    if(row < 0 || row > 9 || column < 0 || column > 9){
        return false;
    }

    return true;
}

void RobotPlayer::printVirtualBoard(){
    for(int i  = 0; i < 10; i++){
        for(int j = 0; j < 10; j++){
            cout << this->virtualBoard[i][j] << " ";
        }
        cout << "\n";
    }
}
