/*
 * SimpleAlgorithm.cpp
 *
 *  Created on: Mar 30, 2016
 *      Author: alonmeytal
 */

#include <map>

#include "Direction.h"
#include "AbstractAlgorithm.h"
#include "Sensor.h"

class SimpleAlgorithm: public AbstractAlgorithm {
	map<string,int> settings;
	AbstractSensor * s;
	int steps;

	SimpleAlgorithm() : settings(NULL), s(NULL), steps(0) {}

	void aboutToFinish(int stepsTillFinishing)
	{
		return;
	}

	Direction step()
	{
		if (s == NULL)
		{
			return Direction::Stay;
		}
		SensorInformation inf = s->sense();
		int direction = rand()%5;
		while (!inf.isWall[direction = rand()%5]);
		steps++;
		return Direction[direction];
	}

	void setConfiguration(map<string, int> config) { settings = config; }

	void setSensor(const AbstractSensor& sensor) { s = sensor; }
};
