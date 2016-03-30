/*
 * username1: alonmeytal
 * ID1: 203972419
 *
 * username2:chenanily
 * ID2:305008864
 */

#include <map>
#include <cstdlib>
#include <ctime>

#include "Direction.h"
#include "AbstractAlgorithm.h"
#include "Sensor.h"

class SimpleAlgorithm: public AbstractAlgorithm {
	map<string,int> settings;
	const AbstractSensor * s;
	int steps;
public:
	SimpleAlgorithm() : s(nullptr), steps(0) { std::srand(std::time(0)); }

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
		int direction;
		SensorInformation inf = s->sense();
		while (!inf.isWall[direction = std::rand()%5]);
		steps++;
		return (Direction)direction;
	}

	void setConfiguration(map<string, int> config) { settings = config; }

	void setSensor(const AbstractSensor& sensor) { s = &sensor; }
};
