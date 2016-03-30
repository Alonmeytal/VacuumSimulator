/*
 * username1: alonmeytal
 * ID1: 203972419
 *
 * username2:chenanily
 * ID2:305008864
 */

#ifndef SENSOR_H_
#define SENSOR_H_

#include "AbstractSensor.h"
#include "House.h"
#include "Point.h"

class Sensor: public AbstractSensor {
	House * house;
	Point * currentLocation;
public:
	Sensor(House * h, Point * location) : house(h), currentLocation(location) { }
	virtual SensorInformation sense() const;
	virtual ~Sensor();
};

#endif /* SENSOR_H_ */
