/*
 * Simulation.h
 *
 *  Created on: Mar 22, 2016
 *      Author: alonmeytal
 */

#ifndef SIMULATION_H_
#define SIMULATION_H_
#include "AbstractAlgorithm.h"
#include "AbstractSensor.h"
#include "Point.h"
#include "Direction.h"

class Simulation
{
	AbstractAlgorithm * algorithm;
	House house;
	map<string, int> * settings;

	int steps; // steps taken so far.
	int batteryMode; // how much charge is left in the robot.
	int dirtCollected; // dirt collected so far.
	bool hasFinished; // flag for when the robot finishes cleaning.
	Point currentLocation; // robot's current location.
	AbstractSensor * sensor;

	Simulation(AbstractAlgorithm * algo, House home, map<string,int> * s) :
		steps(0), batteryMode(s["batteryCapacity"]), dirtCollected(0), hasFinished(false), algorithm(algo), house(home), settings(s), currentLocation(home.dockingPoint), sensor(nullptr)
	{
		// TO-DO : fill other class memebers.
		// sensor(&house, &currentLocation);
	}

	int runStep(); // runs the algorithm for a single step, updating currentLocation, batteryMode, and house dirt-levels.
	// -1 = Failed step, 0 = Normal step, 1 = Finished cleaning, 2 = after finishing (hasFinished = 1 ==> runStep hasn't done anything).
};

#endif /* SIMULATION_H_ */
