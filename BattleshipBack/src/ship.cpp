#include <iostream> 
#include <string>
#include "ship.h"

using namespace std;

// Construtor que inicializa o nome, tamanho e orientações padrão do navio.
Ship::Ship(string name, int size)
: name(name), size(size), hits(0), horizontal(true) {}

// Retorna verdadeiro se o navio foi destruído (todas as posições atingidas).
bool Ship::isDestroyed() {
	return this->hits >= this->size;
}

// Retorna a orientação do navio.
bool Ship::isHorizontal(){
	return this->horizontal;
}

// Alterna a orientação do navio (horizontal/vertical).
void Ship::setOrientation(bool horizontal) {
	this->horizontal = horizontal;
}

void Ship::incrementHits(){
	this->hits++;
}

int Ship::getHits(){
	return this->hits;
}
