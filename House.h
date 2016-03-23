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
	int rows, cols;
	string name, description;
	char ** matrix;
	Point dockingPoint;

	House() :rows(0), cols(0), name("NONAME"), description("NONE"), matrix(nullptr) {};
	House(string fileName);
	void vacuum(int x, int y);
	void print();
	virtual ~House();
};

#endif /* HOUSE_H_ */
