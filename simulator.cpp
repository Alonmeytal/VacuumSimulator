/*
 * simulator.cpp
 *
 *  Created on: Mar 18, 2016
 *      Author: alonmeytal
 */

#include <iostream>
#include <string>
#include <list>

// Interface(s) provided.
#include "Direction.h"
#include "SensorInformation.h"
#include "AbstractSensor.h"
#include "AbstractAlgorithm.h"
// Actual Class(es)
#include "House.h"
// Function Objects
#include "Settings.h"
//#include "HouseReader.h"
//#include "Score.h"

using namespace std;

int main(int argc, char ** argv)
{
	int i;
	// ! get the following command-line arguments : -config -housepath
	// setting up markers for where the arguments are in the command-line string
	// Initialized to -1, if it won't change, then argument was not found.
	int configIndex = -1, housepathIndex = -1;

	string configPath = "./";
	string housesPath = "./";

	if (argc > 1)
	{
		// command-line arguments (one or both) were inserted.
		for (i = 1; i < argc; i++)
		{
			if (argv[i] == "-config")
				configIndex = i + 1;
			else if (argv[i] == "-house_path")
				housepathIndex = i + 1;
		}
		// -config argument was inserted, updating config file path.
		if (configIndex != -1)
			configPath = argv[configIndex];
		// -hosue_path argument was inserted, updating path to house files.
		if (housepathIndex != -1)
			housesPath = argv[housepathIndex];
	}

	// ! load setting from ($configPath)/config.ini

	// declaring setting set;
	map<string, int> settings;
	// loading settings from file (to "set", sent by reference).
	settingsFromFile(setting, configPath + "config.ini");

	// creating houses list
	//list<House> houses;

	// ! read all houses house files from ($housesPath)/*.house
	//HouseReader reader(*houses);
	//reader(housesPath);

	// ! implement a simple algorithm for a cleaning robot.
	// taken from recitation.


	// Import other algorithms.
	list<AbstractAlgorithm> algorithms;

	// ! run a simulation of the algorithm on the house
	/*
	 * initialize scoring table (algorithms * houses)
	 * for each houses:
	 * 	for each algorithm:
	 * 		create simulation instance - run algorithm on house. (return SimulationSteps)
	 * 	calculate scores
	 * 	save score into table
	 */
	// ! return scoring table.

	return 1;
}
