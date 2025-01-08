#include <iostream>
#include "fleet.h"
#include "ship.h"

//Inicializa uma frota conforme padrão.
Fleet::Fleet(){
	this->ships[0] = Ship("Porta-aviões", 6);
	this->ships[1] = Ship("Navio de Guerra", 4);
	this->ships[2] = Ship("Encouraçado", 3);
	this->ships[3] = Ship("Encouraçado", 3);
	this->ships[4] = Ship("Submarino", 1);
}

//Verifica se toda a frota foi detruída ou não.
bool Fleet::isDestroyed(){
	for(int i = 0; i < 5; i++){
		if(!this->ships[i].isDestroyed()){
			return false;
		}
	}
	
	return true;
}
