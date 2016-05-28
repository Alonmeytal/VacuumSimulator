/*
 * username1: alonmeytal
 * ID1: 203972419
 *
 * username2:chenanily
 * ID2:305008864
 */

#ifndef SENSOR_H_
#define SENSOR_H_

#include <memory>

#include "AbstractSensor.h"
#include "House.h"
#include "Point.h"

class Sensor: public AbstractSensor {
	unique_ptr<House> house;
	unique_ptr<Point> currentLocation;
public:
	Sensor(House * h, Point * location) : house(h), currentLocation(location) { }
	virtual SensorInformation sense() const;
	~Sensor() { house.release(); currentLocation.release(); };
};

#endif /* SENSOR_H_ */
