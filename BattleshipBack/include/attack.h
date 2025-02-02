#ifndef ATTACK_H
#define ATTACK_H

#include <iostream>

using namespace std;

class Attack{
	private:
		int row;
		int column;
		Attack* left;
		Attack* right;
	public:
		Attack(){}
		Attack(int, int);
		~Attack(){}

        // Sobrecarga do operador <<
        friend ostream& operator<<(ostream& os, const Attack& attack);
        
		int getRow();
		int getColumn();

		void setLeft(Attack*);
		Attack* getLeft();

		void setRight(Attack*);
		Attack* getRight();
};

#endif