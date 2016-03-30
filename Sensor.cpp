/*
 * username1: alonmeytal
 * ID1: 203972419
 *
 * username2:chenanily
 * ID2:305008864
 */

#include "Sensor.h"
#include "House.h"
#include "Point.h"
#include "Direction.h"
#include <string>

using namespace std;

SensorInformation Sensor::sense() const{
	SensorInformation inf;

	// Assuming the currentLocation is always inside walls, and so moving -/+1 from currentLocation is still in matrix boundaries.
	// Checking the possibility to go up.
	if (house->matrix[currentLocation->getY() + 1][currentLocation->getX()] == 'W')
	{
		inf.isWall[(int)Direction::North] = true;
	}
	else
	{
		inf.isWall[(int)Direction::North] = false;
	}
	// down
	if (house->matrix[currentLocation->getY() - 1][currentLocation->getX()] == 'W')
	{
		inf.isWall[(int)Direction::South] = true;
	}
	else
		inf.isWall[(int)Direction::South] = false;
	// right
	if (house->matrix[currentLocation->getY()][currentLocation->getX() + 1] == 'W')
	{
		inf.isWall[(int)Direction::East] = true;
	}
	else
	{
		inf.isWall[(int)Direction::East] = false;
	}
	// left
	if (house->matrix[currentLocation->getY()][currentLocation->getX() - 1] == 'W')
	{
		inf.isWall[(int)Direction::West] = true;
	}
	else
	{
		inf.isWall[(int)Direction::West] = false;
	}
	// finding out how much dirt is at currentLocation.
	inf.dirtLevel = (house->matrix[currentLocation->getY()][currentLocation->getX()] - '0');

	return inf;
}

Sensor::~Sensor() {
}

