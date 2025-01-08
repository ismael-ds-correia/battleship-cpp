#include <iostream>
#include "position.h"

Position::Position(){
	this->isAttacked = false;
	this->isBlocked = false;
	this->containsShip = false;
} 

void Position::attack(){
	this->isAttacked = true;
}

void Position::block(){
	this->isBlocked = true;
}

void placeShip(){
	this->containsShip = true;
}