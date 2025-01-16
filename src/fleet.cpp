#include <iostream>
#include "fleet.h"
#include "ship.h"

// Inicializa uma frota com os navios padrão.
Fleet::Fleet() {
	this->ships[0] = Ship("Porta-aviões", 6);
	this->ships[1] = Ship("Navio de Guerra", 4);
	this->ships[2] = Ship("Encouraçado", 3);
	this->ships[3] = Ship("Encouraçado", 3);
	this->ships[4] = Ship("Submarino", 1);
}

// Verifica se todos os navios da frota foram destruídos.
bool Fleet::isDestroyed() {
	for (int i = 0; i < 5; i++) {
		if (!this->ships[i].isDestroyed()) { // Se algum navio ainda não foi destruído.
			return false;
		}
	}
	
	return true; // Todos os navios foram destruídos.
}

// Retorna um navio da frota com base em um índice.
Ship& Fleet::getShip(int index){
	return this->ships[index];
}