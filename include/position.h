#ifndef POSITION_H
#define POSITION_H

#include <iostream> 
#include "ship.h"

// Classe que representa uma posição do tabuleiro.
class Position {
	private:
		bool attacked;        // Indica se a posição já foi atacada.
		bool blocked;         // Indica se a posição está bloqueada.
		Ship* shipReference;  // Ponteiro para o navio que ocupa a posição (se existir).
	public:
		Position();          // Construtor padrão.
		~Position(){}        // Destrutor padrão.
		void attack();       // Marca a posição como atacada.
		void block();        // Marca a posição como bloqueada.
		void placeShip(Ship& ship); // Associa um navio à posição.
		Ship* getShipReference();   // Retorna a referência ao navio associado.
		bool isValidPosition();     // Retorna se a posição é válida para posicionamento.
		bool isAttacked();          // Retorna se a posição já foi atacada.
		bool isBlocked();           // Retorna se a posição está bloqueada.
};

#endif
