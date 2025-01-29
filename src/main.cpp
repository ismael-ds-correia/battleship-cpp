#include <iostream>
#include "board.h"
#include "ship.h"
#include "player.h"
#include "Position.h"
#include "fleet.h"

using namespace std;

int main() {
    Ship testShip("test", 3);

    Board testBoard;

    testShip.setOrientation(false);

    testBoard.placeShip(testShip, 2, 3);

    testBoard.print();

    return 0;
}
