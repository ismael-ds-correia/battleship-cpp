#ifndef CIRCULARLINKEDLIST_H
#define CIRCULARLINKEDLIST_H

#include <iostream>
#include "attack.h"

using namespace std;

class CircularLinkedList{
	private:
		Attack* head;
	public:
		CircularLinkedList(){}
		CircularLinkedList(int, int);

		void push(int, int);
		Attack* pop();
		bool contains(int, int);
		void print();

		Attack* getHead();
};

#endif