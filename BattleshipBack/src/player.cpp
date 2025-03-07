#include <iostream>
#include "player.h" 
#include "ship.h"
#include "fleet.h"
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>
#include "board.h"

using namespace std;

Player::Player(string name)
    : name(name), board(), fleet() { // Lista de inicialização.

    if (name.empty()) {
        this->name = "Unknown Player"; // Define um nome padrão caso o nome esteja vazio.
    }
}

bool Player::placeShip(int shipIndex, int line, int column, bool horizontal){
	// Verifica se o índice é válido.
	if(shipIndex < 0 || shipIndex >= 5){
		cout << "\nÍndice inválido!\n";
	}

	// Obtendo o navio a partir da frota.
	Ship& ship = this->fleet.getShip(shipIndex);

	// Define a oritentação do navio.
	ship.setOrientation(horizontal);

	// Verifica se a posição é válida para posicionar.
	if(this->board.checkPosition(ship, line, column)){
		// Posiciona o navio no tabuleiro.
		this->board.placeShip(ship, line, column);
		cout << "\nNavio posicionado!\n";

		return true;
	}else{
		cout << "\nPosição inválida!\n";

		return false;
	}
}

// Método para alterar a posição de um navio já posicionado.
bool Player::repositionShip(int shipIndex, int newLine, int newColumn, bool newHorizontal){
	// Remove o navio do tabuleiro.
	Ship& ship = this->fleet.getShip(shipIndex);
	this->board.removeShip(ship);

	// Tenta reposicionar o navio.
	return this->placeShip(shipIndex, newLine, newColumn, newHorizontal);
}

void Player::positionShipsRandomly() {
    // Lista de todas as posições possíveis (tabuleiro 10x10)
    vector<pair<int, int>> positions;
    for (int row = 0; row < 10; ++row) {
        for (int column = 0; column < 10; ++column) {
            positions.emplace_back(row, column);
        }
    }

    // Embaralhar as posições
    //srand(std::time(0));
    random_shuffle(positions.begin(), positions.end());

    int count = 0; // Contador de navios posicionados
    for (auto& pos : positions) {
        if (count >= 5) break; // Todos os navios foram posicionados

        Ship& currentShip = this->fleet.getShips()[count];

        // Define orientação aleatória para o navio
        bool randomBool = rand() % 2 == 0;
        currentShip.setOrientation(randomBool);
        // Tenta posicionar o navio
        if (this->board.placeShip(currentShip, pos.first, pos.second)) {
            count++; // Incrementa o contador se o navio foi posicionado
        }
    }
}

bool Player::attackOpponent(Board& enemyBoard, int row, int column){
	return enemyBoard.attack(row, column);
}

string Player::getName(){
	return this->name;
}

Board& Player::getBoard(){
	return this->board;
}

void Player::printBoard(){
	this->board.print();
}
/*
void Player::positionShipsRandomly(){
    int row = 0, column = 0, count = 0, i = 0;
    while(count < 5){
        Ship currentShip = this->fleet.getShips()[count];

        srand(std::time(0));
        row = rand()%10;
        column = rand()%10;

        bool randomBool = rand()%2 == 0;
        currentShip.setOrientation(randomBool);

        cout << i++ << "\n";

        if(!this->board.placeShip(currentShip, row, column)){
            continue;
        }

        count++;
    }
}*/
