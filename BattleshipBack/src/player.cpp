#include <iostream>
#include "player.h" 
#include "ship.h"
#include "fleet.h"

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

	// Verifica se a posição é válida para posicionar o navio é válida.
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

string Player::getName(){
	return this->name;
}

Board& Player::getBoard(){
	return this->board;
}