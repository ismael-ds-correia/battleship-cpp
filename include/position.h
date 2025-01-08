#ifndef POSITION_H
#define POSITION_H

#include <iostream> 

class Position{
	private:
		bool isAttacked;
		bool isBlocked;
		bool containsShip;
	public:
		Position();
		~Position(){}
		void attack();
		void block();
		void placeShip();
};

#endif