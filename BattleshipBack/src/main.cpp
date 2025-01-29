#include <iostream>
#include "board.h"
#include "ship.h"
#include "player.h"
#include "Position.h"
#include "fleet.h"

using namespace std;

int main() {
    Player player("eu kkk");

    player.positionShipsRandomly();

    player.getBoard().print();


    return 0;
}
