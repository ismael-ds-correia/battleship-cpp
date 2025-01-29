#ifndef FLEET_H
#define FLEET_H

#include <iostream>
#include "ship.h"

// Classe que representa uma frota de navios.
class Fleet {
	private:
		Ship ships[5]; // Array contendo os 5 navios da frota.
	public:
		Fleet();  // Construtor padrão que inicializa os navios da frota.
		~Fleet(){} // Destrutor padrão.
		bool isDestroyed(); // Verifica se toda a frota foi destruída.
		Ship& getShip(int);
};

#endif
