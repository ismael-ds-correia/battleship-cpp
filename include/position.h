#ifndef POSITION_H
#define POSITION_H

#include <iostream> 
#include "ship.h"

class Position{
	private:
		bool isAttacked;
		bool isBlocked;
		Ship* shipReference;
	public:
		Position();
		~Position(){}
		void attack();
		void block();
		void placeShip(Ship& ship);
		Ship* getShipReference();
};

#endif