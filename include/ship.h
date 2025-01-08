#ifndef SHIP_H
#define SHIP_H

#include <iostream>
#include <string>

using namespace std;

class Ship{
	private:
		string name; 		//Refere-se ao nome do navio (battleship, submarine, frigate, etc.).
		int size;			//Quantidade de posições ocupadas.
		int hits;			//Quantidade de posições atingidas.
	public:
		Ship(string, int);
		~Ship(){}
		bool isDestroyed();
};

#endif