#include "Simulation.h"
#include "Sensor.h"
#include "House.h"
#include "Point.h"
#include "Direction.h"

using namespace std;

int Simulation::runStep(){
	if (this.hasFinished) {
		return 2;
	}
	if (batteryMode == 0) {
		return -1;
	}
	SensorInformation currentSensorInfo;
	currentSensorInfo = sensor.Sense();
	Direction wantedDir = algorithm::Step();

	if (currentSensorInfo->isWall[wantedDir]) {
		return -1;
	}
	//there is enough battery and next step is not a wall, so we are moving
	if (currentSensorInfo->dirtLevel > 0){
		house:vacuum(currentLocation->x, currentLocation->y);
	}
	steps++;
	batteryMode--;
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
	}
	if (house->matrix[currentLocation->y][currentLocation->x] == 'D' && dirtCollected==house.dirt){
		this.hasFinished = true;
		return 1;
	}
	return 0;
}