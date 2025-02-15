#include <iostream>
#include "board.h"
#include "ship.h"
#include "player.h"
#include "Position.h"
#include "fleet.h"
#include <ctime>
#include "robotplayer.h"
#include "circularlinkedlist.h"

using namespace std;

int main() {
    //srand(std::time(0));
    CircularLinkedList list(2, 3);
    list.push(3, 5);
    list.push(2, 5);
    list.push(1, 5);
    list.push(1, 5);
    list.push(5, 5);
    list.push(7, 5);
    list.push(9, 5);

    cout << *list.pop();

    list.print();

    cout << *list.pop();

    cout << "\n";

    Player player("eu kkk");

    player.positionShipsRandomly();

    player.getBoard().print();

    return 0;
}
