/*
 * username1: alonmeytal
 * ID1: 203972419
 *
 * username2:chenanily
 * ID2:305008864
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
