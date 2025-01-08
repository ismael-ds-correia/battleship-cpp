#ifndef FLEET_H
#define FLEET_H

#include <iostream>
#include "ship.h"

class Fleet{
	private:
		Ship ships[5];
	public:
		Fleet();
		~Fleet(){}
		bool isDestroyed();
};

#endif