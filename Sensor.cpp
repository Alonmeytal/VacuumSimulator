/*
 * Sensor.cpp
 *
 *  Created on: Mar 22, 2016
 *      Author: alonmeytal
 */

#include "Sensor.h"
#include "House.h"
#include "Point.h"
#include <string>

using namespace std;

SensorInformation Sensor::sense() {
	SensorInformation * inf = new SensorInformation;

	// Assuming the currentLocation is always inside walls, and so moving -/+1 from currentLocation is still in matrix boundaries.
	// Checking the possibility to go up.
	if (house->matrix[currentLocation->y + 1][currentLocation->x] == 'W')
	{
		inf->isWall[Direction::North] = true;
	}
	else
	{
		inf->isWall[Direction::North] = false;
	}
	// down
	if (house->matrix[currentLocation->y - 1][currentLocation->x] == 'W')
	{
		inf->isWall[Direction::South] = true;
	}
	else
		inf->isWall[Direction::South] = false;
	// right
	if (house->matrix[currentLocation->y][currentLocation->x + 1] == 'W')
	{
		inf->isWall[Direction::East] = true;
	}
	else
	{
		inf->isWall[Direction::East] = false;
	}
	// left
	if (house->matrix[currentLocation->y][currentLocation->x - 1] == 'W')
	{
		inf->isWall[Direction::West] = true;
	}
	else
	{
		inf->isWall[Direction::West] = false;
	}
	// finding out how much dirt is at currentLocation.
	inf->dirtLevel = atoi(house->matrix[currentLocation->y][currentLocation->x]);

	return inf;
}

Sensor::~Sensor() {
}

