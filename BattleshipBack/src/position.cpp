#include <iostream>
#include "position.h"
#include "ship.h"

// Construtor que inicializa a posição com valores padrão.
Position::Position() {
	this->attacked = false;
	this->blocked = false;
	this->shipReference = nullptr;
} 

// Marca a posição como atacada.
void Position::attack() {
	this->attacked = true;
}

// Marca a posição como bloqueada.
void Position::block() {
	this->blocked = true;
}

// Desbloqueia uma posição.
void Position::unlock(){
	this->blocked = false;
}

// Associa um navio à posição.
void Position::placeShip(Ship& ship) {
	this->shipReference = &ship;
}

// Desassocia a possição a um navio.
void Position::removeShip(){
	this->shipReference = nullptr;
}

// Retorna se a posição já foi atacada.
bool Position::isAttacked() {
	return this->attacked;
}

// Retorna se a posição está bloqueada.
bool Position::isBlocked() {
	return this->blocked;
}

// Retorna o ponteiro para o navio associado à posição.
Ship* Position::getShipReference() {
	return this->shipReference;
}

// Retorna se a posição é válida para posicionamento de um navio.
bool Position::isValidPosition() {
	return !this->attacked && !this->blocked;
}
