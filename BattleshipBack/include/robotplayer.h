#ifndef ROBOTPLAYER_H
#define ROBOTPLAYER_H
#include <iostream>
#include "player.h"
#include "board.h"
#include <queue>
#include <vector>
#include <utility>
#include <thread>
#include <chrono>

using namespace std;

class RobotPlayer :  public Player{
    private:
        int virtualBoard[10][10];
        queue<pair<int, int>> priorityQueue;
        int alertAttack=0;
        int sizeOfEnemyShips[];
        std::pair<int, int> locateShipStart(Board&, int, int);
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
        bool shouldAttackStrategicPositions();
        void planStrategicAttack(Board& enemyBoard);
        bool searchVertically(int size);
        bool searchHorizontally(int size);
        int sizeOfTheNextShip();
        void oneLessShip(int sizeOfShipDestroyed);
};

#endif
