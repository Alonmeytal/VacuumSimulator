/*
 * username1: alonmeytal
 * ID1: 203972419
 *
 * username2:chenanily
 * ID2:305008864
 */

#ifndef __ABSTRACT_SENSOR__H_
#define __ABSTRACT_SENSOR__H_

#include "SensorInformation.h"

class AbstractSensor 
{ 
public: 
	virtual ~AbstractSensor() {}
    // returns the sensor's information of the current location of the robot 
    virtual SensorInformation sense() const = 0; 
};

#endif //__ABSTRACT_SENSOR__H_
