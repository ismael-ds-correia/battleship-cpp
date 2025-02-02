#include <iostream> 
#include "circularlinkedlist.h"
#include "attack.h"

CircularLinkedList::CircularLinkedList(int row, int column){
	this->head = new Attack(row, column);
	this->head->setLeft(this->head);
	this->head->setRight(this->head);
}

void CircularLinkedList::push(int row, int column){
	if(contains(row, column)){
		return;
	}

	Attack* newAttack = new Attack(row, column);

	newAttack->setLeft(this->head->getLeft());
	newAttack->setRight(this->head);

	this->head->getLeft()->setRight(newAttack);
	this->head->setLeft(newAttack);

	this->head = newAttack;
}

Attack* CircularLinkedList::pop() {
    if (this->head == nullptr) {
        return nullptr;
    }

    Attack* toRemove = this->head;

    if (this->head->getLeft() == this->head && this->head->getRight() == this->head) {
        this->head = nullptr;
    } else {
        Attack* left = this->head->getLeft();
        Attack* right = this->head->getRight();

        left->setRight(right);
        right->setLeft(left);

        this->head = right;
    }

    toRemove->setLeft(nullptr);
    toRemove->setRight(nullptr);

    return toRemove;
}


bool CircularLinkedList::contains(int row, int column){
	   if (this->head == nullptr) {
        return false;
    }

	if(this->head->getRow() == row && this->head->getColumn() == column){
		return true;
	}

	Attack* currentAttack = this->head->getRight();
	while (currentAttack != this->head) {
        if (currentAttack->getRow() == row && currentAttack->getColumn() == column) {
            return true;
        }

        currentAttack = currentAttack->getRight();
    }

    return false;
}

void CircularLinkedList::print(){
	if(this->head == nullptr){
		return;
	}

	Attack* currentAttack = this->head->getRight();
    do {
        cout << *currentAttack << "\n"; // Desreferencia o ponteiro para imprimir o objeto.
        currentAttack = currentAttack->getRight(); // Vai para o próximo nó.
    } while (currentAttack != this->head); // Para quando voltar ao início.

    cout << *currentAttack << "\n";
}

Attack* CircularLinkedList::getHead(){
	return this->head;
}