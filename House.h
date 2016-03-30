/*
 * House.h
 *
 *  Created on: Mar 22, 2016
 *      Author: alonmeytal
 */

#ifndef HOUSE_H_
#define HOUSE_H_
#include <string>
#include "Point.h"

using namespace std;

class House {
public:
	int rows, cols; // house size (rows and columns).
	int dirt; // sum of dirt in the house (to begin with).
	string name, description;
	char ** matrix; // house matrix[rows][cols].
	Point dockingPoint; // Point representing the co-ordinates of 'D' in the house.

	// empty constructor.
	House() :rows(0), cols(0), dirt(0), name("NONAME"), description("NONE"), matrix(nullptr), dockingPoint(Point(0,0)) {};
	// copy constructor.
	House(const House& otherHouse);
	// from *.house file constructor.
	House(string fileName);

	void vacuum(int x, int y); // lower the dirt level at [x,y].
	void print(); // print the house to std::cout.
	virtual ~House(); // deconstructor.
};

#endif /* HOUSE_H_ */
