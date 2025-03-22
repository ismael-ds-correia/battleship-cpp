#ifndef ROBOTPLAYER_H
#define ROBOTPLAYER_H
#include <iostream>
#include "player.h"
#include "board.h"
#include <queue>
#include <vector>
#include <utility>

using namespace std;

class RobotPlayer :  public Player{
	private:
		int virtualBoard[10][10];
		queue<pair<int, int>> priorityQueue;
		int alertAttack=0;
	public:
		RobotPlayer();
		~RobotPlayer(){}
		void attack(Board&);
		void addToPriorityQueue(int, int);
		void adjustStrategy(Board&, int, int);
		bool isValid(int, int);
		bool isValidForTesting(int, int);
		void attackNeighbors(int, int);
		void wreckedShipAdjustment(Board&, int, int);
		void clearProrityQueue();
		void discoverDirectionAndAdd(Board&, int, int);
		void printVirtualBoard();
};

#endif