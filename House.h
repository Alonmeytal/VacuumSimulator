/*
 * username1: alonmeytal
 * ID1: 203972419
 *
 * username2:chenanily
 * ID2:305008864
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
	string name;
	int maxSteps;
	char ** matrix; // house matrix[rows][cols].
	Point dockingPoint; // Point representing the co-ordinates of 'D' in the house.

	// empty constructor.
	House() :rows(0), cols(0), dirt(0), name("NONAME"), maxSteps(0), matrix(nullptr), dockingPoint(Point(0,0)) {};
	// copy constructor.
	House(const House& otherHouse);
	// from *.house file constructor.
	House(string fileName);

	bool isValid();
	void vacuum(int x, int y); // lower the dirt level at [x,y].
	void print(); // print the house to std::cout.
	virtual ~House(); // deconstructor.
};

#endif /* HOUSE_H_ */
