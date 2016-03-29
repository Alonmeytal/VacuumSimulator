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
#include "Score.h"

using namespace std;

int main(int argc, char ** argv)
{
	int i, j;
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
	settingsFromFile(settings, configPath + "config.ini");

	// creating houses list
	list<House> houses;

	// ! read all houses house files from ($housesPath)/*.house
	HouseReader reader(*houses);
	reader(housesPath);

	// ! implement a simple algorithm for a cleaning robot.
	// taken from recitation.

	// Import other algorithms.
	list<AbstractAlgorithm> algorithms;

	// load all algorithms to list;

	// ! run a simulation of the algorithm on the house
	list<Simulation> simulationsList;

	// setting up simulation steps counting table : (house, algorithm) for scoring purposes.
	int ** simulationSteps = new int*[houses.size()];

	for (i = 0; i < houses.size(); i++)
	{
		simulationSteps[i] = new int[algorithms.size()];
	}
	for (i = 0; i < houses.size(); i++)
	{
		for (j = 0; j < algorithms.size(); j++)
		{
			simulationSteps[i][j] = 0;
		}
	}

	// for each house;
	int stepsTaken, stepsFromWinner, failedRobots;
	bool winnerFound;
	i = 0;
	for (House currentHouse = houses.begin(); currentHouse != houses.end(); currentHouse++)
	{
		// for each algorithm;
		for (AbstractAlgorithm currentAlgorithm = algorithms.begin(); currentAlgorithm != algorithms.end(); currentAlgorithm++)
		{
			// create a simulation instance that will run currentAlgorithm on currentHouse;
			simulationsList.push_back(Simulation(currentAlgorithm, currentHouse, &settings));
		}
		// should now run all algorithms on house, in a "round-robin" fashion.
		stepsTaken = 0;
		stepsFromWinner = 0;
		failedRobots = 0;
		winnerFound = false;
		// iterate while stepsTaken <= MaxSteps (setting) OR MaxStepsAfterWinner reached  AND not all robots made a mistake.
		while (((stepsFromWinner <= settings["MaxStepsAfterWinner"]) || (stepsTaken <= settings["MaxSteps"])) && (failedRobots < algorithms.size()))
		{
			int stepResult;
			j = 0;
			// making each algorithm make a single step.
			for (Simulation sim = simulationsList.begin(); sim != simulationsList.end(); sim++)
			{
				stepResult = sim.runStep();
				switch (stepResult)
				{
				case -1 :
					// algorithm made a mistake.
					simulationSteps[i][j] = -1;
					cout << "algorithm " << j << " has made a mistake." << endl;
					failedRobots++;
					break;
				case 1 :
					// algorithm finished.
					if (winnerFound == false)
					{
						winnerFound = true;
					}
					simulationSteps[i][j] = stepsTaken;
					break;
				default :
					// algorithm either returned 0 = nothing, or 2 = already done, didn't move.
					continue;
				}
				j++;
			}
			if (winnerFound)
			{
				// winner was found, now counting legal steps till MaxStepsAfterWinner.
				stepsFromWinner++;
			}
			// either way, counting steps made.
			stepsTaken++;
		}

		// finished itrating over currentHouse.
		// looking for unfinished algorithms, setting their steps to simulationSteps.
		for (i = 0; i < algorithms.size(); i++)
		{
			if (simulationSteps[i][j] == 0)
			{
				// robot did not finish
				simulationSteps[i][j] = stepsTaken;
			}
		}

		// scoring the robots for currentHouse.


		// emptying simulationsList for next house (next round of simulations).
		simulationsList.clear();
		i++;
	}

	// freeing simlationSteps table.
	for (i = 0; i < houses.size(); i ++)
	{
		delete [] simulationSteps[i];
	}
	delete [] simulationSteps;

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
