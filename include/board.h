#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include "position.h"
#include "ship.h"

// Classe que representa o tabuleiro do jogo.
class Board {
	private:
		Position positions[10][10];          // Matriz que representa as posições do tabuleiro.
		void blockPositions(Ship&, int, int); // Bloqueia posições adjacentes a um navio.
		bool checkPosition(Ship&, int, int);  // Verifica se um determinado navio pode ser
									 		 // posicionado em uma determinada posição.
	public:
		Board(){}  // Construtor padrão
		~Board(){} // Destrutor padrão
		void placeShip(Ship&, int, int); // Posiciona um navio no tabuleiro.
		void attack(int, int);          // Interface para que o oponente ataque uma posição específica.
		void print();                   // Exibe o estado atual do tabuleiro.
		Position (&getPositions())[10][10]; // Retorna uma referência para a matriz de posições.
};

#endif
