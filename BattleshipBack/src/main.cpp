#include <iostream>
#include "board.h"
#include "ship.h"
#include "player.h"
#include "Position.h"
#include "fleet.h"
#include <ctime>

using namespace std;

int main() {
    srand(std::time(0));
    Player player("eu kkk");
    Player enemy("enemy");

    player.positionShipsRandomly();
    player.getBoard().print();
    cout << "\n";

    enemy.positionShipsRandomly();
    enemy.getBoard().print();
    player.attackOpponent(enemy.getBoard(), 1, 1);

    //player.getBoard().print();

    cout << "\n";

    //enemy.getBoard().print();


    return 0;
}
