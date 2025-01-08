#include <iostream>
#include "position.h"
#include "ship.h"

Position::Position(){
	this->isAttacked = false;
	this->isBlocked = false;
	this->shipReference = nullptr;
} 

void Position::attack(){
	this->isAttacked = true;
}

void Position::block(){
	this->isBlocked = true;
}

void Position::placeShip(Ship& ship){
	this->shipReference = &ship;
}

Ship* Position::getShipReference(){
	return this->shipReference;
}