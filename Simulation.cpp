/*
 * username1: alonmeytal
 * ID1: 203972419
 *
 * username2:chenanily
 * ID2:305008864
 */

#include "Simulation.h"
#include "Sensor.h"
#include "House.h"
#include "Point.h"
#include "Direction.h"

using namespace std;

int Simulation::runStep(){
	if (this->hasFinished) {
		return 2;
	}
	if (batteryMode < settings->find("BatteryConsumptionRate")->second) {
		return -1;
	}
	SensorInformation currentSensorInfo;
	currentSensorInfo = sensor->sense();
	Direction wantedDir = algorithm->step();

	if (currentSensorInfo.isWall[(int)wantedDir]) {
		return -1;
	}
	//there is enough battery and next step is not a wall, so we are moving
	if (currentSensorInfo.dirtLevel > 0){
		house.vacuum(currentLocation.getX(), currentLocation.getY());
	}
	steps++;
	batteryMode -= settings->find("BatteryConsumptionRate")->second;
	dirtCollected++;
	switch (wantedDir)
	{
		case Direction::North:
			currentLocation.up();
			break;
		case Direction::South:
			currentLocation.down();
			break;
		case Direction::East:
			currentLocation.right();
			break;
		case Direction::West:
			currentLocation.left();
			break;
		case Direction::Stay: //we're doing nothing but just a reminder that is an option
			break;
	}
	if (house.matrix[currentLocation.getY()][currentLocation.getX()] == 'D' && dirtCollected==house.dirt){
		this->hasFinished = true;
		return 1;
	}
	return 0;
}
