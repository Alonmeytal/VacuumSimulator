/*
 * username1: alonmeytal
 * ID1: 203972419
 *
 * username2:chenanily
 * ID2:305008864
 */
#ifndef SIMULATION_H_
#define SIMULATION_H_

#include <iostream>
#include <string>
#include <memory>

#include "AbstractAlgorithm.h"
#include "AbstractSensor.h"
#include "Sensor.h"
#include "Point.h"
#include "Direction.h"
#include "House.h"

using namespace std;

class Simulation
{
	unique_ptr<AbstractAlgorithm> algorithm;
	House house;
	map<string, int> * settings;

	Point currentLocation; // robot's current location.
	AbstractSensor * sensor;
public:
	int steps; // steps taken so far.
	int batteryMode; // how much charge is left in the robot.
	int dirtCollected; // dirt collected so far.
	bool hasFinished; // flag for when the robot finishes cleaning.

	Simulation(unique_ptr<AbstractAlgorithm> algo, House home, map<string,int> * s) :
		 algorithm(std::move(algo)), house(home), settings(s), currentLocation(home.dockingPoint), sensor(nullptr), steps(0), dirtCollected(0), hasFinished(false)
	{
		// TO-DO : fill other class memebers.
		//algorithm = make_unique(algo);
		batteryMode = s->find("BatteryCapacity")->second;
		sensor = new Sensor(&house, &currentLocation);
		cout << "here" << endl;
		algorithm->setSensor(*sensor);
		cout << "there" << endl;
	}

	int runStep(); // runs the algorithm for a single step, updating currentLocation, batteryMode, and house dirt-levels.
	// -1 = Failed step, 0 = Normal step, 1 = Finished cleaning, 2 = after finishing (hasFinished = 1 ==> runStep hasn't done anything).
};

#endif /* SIMULATION_H_ */
