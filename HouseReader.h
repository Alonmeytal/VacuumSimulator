/*
 * HouseReader.h
 *
 *  Created on: Mar 24, 2016
 *      Author: alonmeytal
 */

#ifndef HOUSEREADER_H_
#define HOUSEREADER_H_
#include "House.h"

class HouseReader {
	list<House> lst;
public:
	HouseReader(list<House>& houses) : lst(houses) {}
	void operator()(string housesPath);
	virtual ~HouseReader();
};

#endif /* HOUSEREADER_H_ */
