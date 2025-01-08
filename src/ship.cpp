#include <iostream> 
#include <string>
#include "ship.h"

using namespace std;

Ship::Ship(string name, int size) 
: name(name), size(size), destroyed(false), hits(0){}

bool Ship::isDestroyed(){
	return hits >= size;   //O navio está detruído quando todas as suas posições foram atacadas.
}