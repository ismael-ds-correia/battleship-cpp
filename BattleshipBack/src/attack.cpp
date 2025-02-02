#include <iostream>
#include "attack.h" 

Attack::Attack(int row, int column){
	this->row = row;
	this->column = column;
	this->left = nullptr;
	this->right = nullptr;
}

// Implementação da sobrecarga do operador <<
ostream& operator<<(ostream& os, const Attack& attack) {
    os << "(" << attack.row << ", " << attack.column << ")";
    return os;
}

Attack* Attack::getLeft(){
	return this->left;
}

void Attack::setLeft(Attack* newLeft){
	this->left = newLeft;
}

Attack* Attack::getRight(){
	return this->right;
}

void Attack::setRight(Attack* newRight){
	this->right = newRight;
}

int Attack::getRow(){
	return this->row;
}

int Attack::getColumn(){
	return this->column;
}