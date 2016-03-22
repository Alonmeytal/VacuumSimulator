/*
 * Simulation.h
 *
 *  Created on: Mar 22, 2016
 *      Author: alonmeytal
 */

#ifndef SIMULATION_H_
#define SIMULATION_H_

class Simulation
{
	AbstractAlgorithm algorithm;
	House house;
	Settings * set;

	int steps; // steps taken so far.
	int batteryMode; // how much charge is left in the robot.
	bool hasFinished; // flag for when the robot finishes cleaning.
	point currentLocation; // robot's current location.
	AbstractSensor sesnor;

	Simulation(AbstractAlgoritm algo, House home, Settings s) : algorithm(algo), house(home), set(s) {
		// TO-DO : fill other class memebers.
		// sensor(&house, &currentLocation);
	}

	int runStep(); // runs the algorithm for a single step, updating currentLocation, batteryMode, and house dirt-levels.
	// -1 = Failed step, 0 = Normal step, 1 = Finished cleaning, 2 = after finishing (hasFinished = 1 ==> runStep hasn't done anything).
};

#endif /* SIMULATION_H_ */
