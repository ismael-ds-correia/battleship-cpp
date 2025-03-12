#ifndef SHIP_H
#define SHIP_H

#include <iostream>
#include <string>

using namespace std;

// Classe que representa um navio no jogo.
class Ship {
	private:
		string name; 		// Nome do navio (battleship, submarine, etc.).
		int size;			// Quantidade de posições ocupadas pelo navio.
		int hits;			// Quantidade de posições do navio atingidas.
		bool horizontal;    // Indica se o navio está orientado horizontalmente.
	public:
		Ship(){} // Construtor padrão.
		Ship(string, int); // Construtor que inicializa o nome e tamanho do navio.
		~Ship(){} // Destrutor padrão.
		bool isDestroyed();    // Retorna se o navio foi destruído (todas as posições atingidas).
		bool isHorizontal();   // Retorna a orientação do navio (horizontal ou vertical).
		int getSize(){return size;} // Retorna o tamanho do navio.
        int getShipSize();      //problemas no Qt, tive que adicionar este outro getter
        void setOrientation(bool); // Altera a orientação do navio.
		void incrementHits();
        int getHits();
};

#endif
