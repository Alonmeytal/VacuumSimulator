/*
 * Sensor.h
 *
 *  Created on: Mar 22, 2016
 *      Author: alonmeytal
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
	virtual SensorInformation sense();
	virtual ~Sensor();
};

#endif /* SENSOR_H_ */
