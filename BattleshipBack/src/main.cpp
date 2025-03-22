#include <iostream>
#include "board.h"
#include "ship.h"
#include "player.h"
#include "Position.h"
#include "fleet.h"
#include <ctime>
#include "robotplayer.h"

using namespace std;

int main() {
    srand(std::time(0));
    Player player("eu kk");
    RobotPlayer robot;

    player.positionShipsRandomly();

    for(int i = 0; i < 15; i++){
        robot.attack(player.getBoard());

        player.printBoard();
        cout << "\n";
        robot.printVirtualBoard();
    }

    //player.printBoard();
    return 0;
}
