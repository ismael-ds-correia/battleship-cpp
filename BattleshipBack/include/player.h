#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <string>
#include "board.h"
#include "fleet.h"

class Player{
	private:
		string name;
		Board board;
		Fleet fleet;
		bool placeShip(int, int, int, bool);
	public:
		Player(string);
		~Player(){}
		string getName();
		Board& getBoard();
		void placeFleet();
		bool repositionShip(int, int, int, bool);
		void positionShipsRandomly();
		bool attackOpponent(Board&, int, int);
};

#endif