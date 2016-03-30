/*
 * username1: alonmeytal
 * ID1: 203972419
 *
 * username2:chenanily
 * ID2:305008864
 */

#include <iostream>
#include <string>
#include <list>
#include <array>
#include <algorithm>
#include <cstring>
using namespace std;

// Interface(s) provided.
#include "Direction.h"
#include "SensorInformation.h"
#include "AbstractSensor.h"
#include "AbstractAlgorithm.h"
// Actual Class(es)
#include "House.h"
#include "SimpleAlgorithm.cpp"
#include "Simulation.h"
// Function Objects
#include "Settings.h"
#include "HouseReader.h"
#include "Score.h"

int main(int argc, char ** argv)
{
	int i, j;
	// ! get the following command-line arguments : -config -housepath
	// setting up markers for where the arguments are in the command-line string
	// Initialized to -1, if it won't change, then argument was not found.
	int configIndex = -1, housepathIndex = -1;

	string configPath = "./";
	string housesPath = "./";
	string strConfig = "-config";
	string strHouse = "-house_path";
	if (argc > 1)
	{
		// command-line arguments (one or both) were inserted.
		for (i = 1; i < argc; i++)
		{
			if (strConfig.compare(argv[i]) == 0)
				configIndex = i + 1;
			else if (strHouse.compare(argv[i]) == 0)
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
	//HouseReader reader(*houses);
	//reader(housesPath);
	// Hard coded house
	House coded;
	coded.name = "Simple1";
	coded.description = "2 Bedrooms + Kitchen Isle";
	coded.rows = 8;
	coded.cols = 10;
	coded.matrix = new char*[8];
	for (i = 0; i < coded.rows; i++)
	{
		coded.matrix[i] = new char[coded.cols];
	}
	strcpy(coded.matrix[0],"WWWWWWWWWW"); //{'W','W','W','W','W','W','W','W','W','W'};//"WWWWWWWWWW";
	strcpy(coded.matrix[1],"W22  DW59W");// = {'W','2','2',' ',' ','D','W','5','9','W'};
	strcpy(coded.matrix[2],"W  W 1119W"); // = {'W',' ',' ','W',' ','1','1','1','9','W'};
	strcpy(coded.matrix[3],"W WWW3WW W"); // = {'W',' ','W','W','W','3','W','W',' ','W'};
	strcpy(coded.matrix[4],"W6   3W  W"); // = {'W','6',' ',' ',' ','3','W',' ',' ','W'};
	strcpy(coded.matrix[5],"W78W  W  W"); // = {'W','7','8','W',' ',' ','W',' ',' ','W'};
	strcpy(coded.matrix[6],"W99W  W  W"); // = {'W','9','9','W',' ',' ','W',' ',' ','W'};
	strcpy(coded.matrix[7],"WWWWWWWWWW"); // = {'W','W','W','W','W','W','W','W','W','W'};

	//coded.matrix = {"WWWWWWWWWW","W22  DW59W","W  W 1119W","W WWW3WW W","W6   3W  W","W78W  W  W","W99W  W  W","WWWWWWWWWW"};
	coded.dockingPoint = Point(5,2);
	if (coded.isValid())
	{
		houses.push_back(coded);
	}

	int numOfHouses = houses.size();
	// ! implement a simple algorithm for a cleaning robot.
	// taken from recitation.

	// Import other algorithms.
	list<AbstractAlgorithm*> algorithms;

	// load all algorithms to list;
	SimpleAlgorithm simpson;
	algorithms.push_back(&simpson);

	int numOfAlgorithms = algorithms.size();

	// ! run a simulation of the algorithm on the house
	list<Simulation> simulationsList;

	// setting up simulation steps counting table : (house, algorithm) for scoring purposes.
	int ** simulationSteps = new int*[numOfHouses];
	// also, setting up simulation scoring table.
	int ** simulationScores = new int*[numOfHouses];

	for (i = 0; i < numOfHouses; i++)
	{
		simulationSteps[i] = new int[numOfAlgorithms];
		simulationScores[i] = new int[numOfAlgorithms];
	}
	// initializing counting table values.
	for (i = 0; i < numOfHouses; i++)
	{
		for (j = 0; j < numOfAlgorithms; j++)
		{
			simulationSteps[i][j] = 0;
		}
	}

	// for each house;
	int stepsTaken, winnerSteps, stepsFromWinner, failedRobots, actualPlaceInCompetition, winnersThisRound;
	bool winnerFound, finishedThisRound;
	i = 0;
	for (list<House>::iterator currentHouse = houses.begin(); currentHouse != houses.end(); ++currentHouse)
	{
		// for each algorithm;
		for (list<AbstractAlgorithm*>::iterator currentAlgorithm = algorithms.begin(); currentAlgorithm != algorithms.end(); ++currentAlgorithm)
		{
			// create a simulation instance that will run currentAlgorithm on currentHouse;
			simulationsList.push_back(Simulation(*currentAlgorithm, *currentHouse, &settings));
		}

		int * positionInCompetition = new int[numOfAlgorithms];

		// should now run all algorithms on house, in a "round-robin" fashion.
		stepsTaken = 0; // how many steps has the simulation done so far.
		stepsFromWinner = 0; // simulation steps counting from the first winner.
		winnerSteps = -1;
		failedRobots = 0; // number of robot who has made a mistake.
		winnerFound = false; // flag for if finding a winner.
		actualPlaceInCompetition = 1; // counter for place in competition.
		// iterate while stepsTaken <= MaxSteps (setting) OR MaxStepsAfterWinner reached  AND not all robots made a mistake.
		while (((stepsFromWinner <= settings["MaxStepsAfterWinner"]) || (stepsTaken <= settings["MaxSteps"])) && (failedRobots < numOfAlgorithms))
		{
			finishedThisRound = false;
			winnersThisRound = 0;
			int stepResult;
			j = 0; // algorithm number.
			// making each algorithm make a single step.
			for (list<Simulation>::iterator sim = simulationsList.begin(); sim != simulationsList.end(); ++sim)
			{
				// make sure currentAlgrotim didn't make a mistake and thus should make another step.
				if (simulationSteps[i][j] != -1)
				{
					stepResult = (*sim).runStep();
					switch (stepResult)
					{
					case -1 :
						// algorithm made a mistake.
						simulationSteps[i][j] = -1;
						cout << "algorithm " << j << " has made a mistake." << endl;
						failedRobots++;
						positionInCompetition[j] = 10;
						break;
					case 1 :
						// algorithm finished.
						if (winnerFound == false)
						{
							// first winner found
							winnerFound = true; // flagging that a winner was found.
							winnerSteps = stepsTaken; // marking the winner's steps for scoring reference.
						}
						finishedThisRound = true; // flagging that at least one
						positionInCompetition[j] = actualPlaceInCompetition; // saving the place in competition the algorithm got.
						winnersThisRound++; // counting the number of algorithms who finished this round.
						simulationSteps[i][j] = stepsTaken; // remembering how many steps it took the algorithm to finish.
						break;
					default :
						// algorithm either returned 0 = nothing, or 2 = already done, didn't move.
						continue;
					}
				}
				j++;
			}
			if (finishedThisRound)
			{
				// at least one winner was found this round, place in competition should be increased.
				actualPlaceInCompetition += winnersThisRound; 	// if X robot finished this round, they all got ($actualPlaceInCompetition) place,
																// next one would get $actualPlaceInCompetition + X.
			}
			if (winnerFound)
			{
				// winner was found, now counting legal steps till MaxStepsAfterWinner.
				stepsFromWinner++;
			}
			// either way, counting steps made.
			stepsTaken++;
		}

		// finished iterating over currentHouse.
		// looking for unfinished algorithms, setting their steps to simulationSteps.
		for (j = 0; j < numOfAlgorithms; j++)
		{
			if (simulationSteps[i][j] <= 0)
			{
				// robot did not finish
				simulationSteps[i][j] = stepsTaken;
				positionInCompetition[j] = 10;
			}
		}
		// if winner was not found, winnerSteps should be simulationSteps (stepsTaken).
		if (!winnerFound)
		{
			winnerSteps = stepsTaken;
		}

		// scoring the robots for currentHouse.
		j = 0;
		for (list<Simulation>::iterator sim = simulationsList.begin();sim != simulationsList.end(); ++sim)
		{
			simulationScores[i][j] = score(positionInCompetition[j] == 10 ? 10 : std::min(4,positionInCompetition[j]), winnerSteps,
					simulationSteps[i][j], (*currentHouse).dirt, (*sim).dirtCollected,  (*sim).hasFinished);
			j++;
		}

		delete [] positionInCompetition;
		// emptying simulationsList for next house (next round of simulations).
		simulationsList.clear();
		i++;
	}

	// print scoring function.
	for (i = 0; i < numOfAlgorithms; i++)
	{
		// for each algorithm.
		j = 0;
		for (list<House>::iterator h = houses.begin(); h != houses.end(); ++h)
		{
			// for each house.
			// print "[<House Short Name>]\t<Score>\n".
			//cout << "[" << h.name << "]\t" << simulationScores[j][i] << endl;
			cout << simulationScores[j][i] << endl; // required for Targil 1 only.
			j++;
		}
	}

	// freeing simlationSteps table.
	for (i = 0; i < numOfHouses; i++)
	{
		delete [] simulationSteps[i];
		delete [] simulationScores[i];
	}
	delete [] simulationSteps;
	delete [] simulationScores;

	return 1;
}
