/*
 * username1: alonmeytal
 * ID1: 203972419
 *
 * username2:chenanily
 * ID2:305008864
 */

#include <iostream>
#include <iomanip>
#include <string>
#include <list>
#include <array>
#include <algorithm>
#include <cstring>

using namespace std;

#include "Simulator.h"

void Simulator::run() {
	int i, j;
	// count houses and algorithms for future reference.
	int numOfHouses = houses.size();
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
	for (House& currentHouse : houses)
	{
		// for each algorithm;
		for (AbstractAlgorithm * currentAlgorithm : algorithms)
		{
			// create a simulation instance that will run currentAlgorithm on currentHouse;
			simulationsList.emplace_back(currentAlgorithm, currentHouse, &settings);
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
		while (((stepsFromWinner <= settings["MaxStepsAfterWinner"]) && (stepsTaken <= currentHouse.maxSteps)) && (failedRobots < numOfAlgorithms))
		{
			finishedThisRound = false;
			winnersThisRound = 0;
			int stepResult;
			j = 0; // algorithm number.
			// making each algorithm make a single step.
			for (Simulation& sim : simulationsList)
			{
				// make sure currentAlgrotim didn't make a mistake and thus should make another step.
				if (simulationSteps[i][j] != -1)
				{
					stepResult = sim.runStep();
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
		for (Simulation& sim : simulationsList)
		{
			simulationScores[i][j] = score(positionInCompetition[j] == 10 ? 10 : std::min(4,positionInCompetition[j]), winnerSteps,
					simulationSteps[i][j], currentHouse.dirt, sim.dirtCollected,  sim.hasFinished);
			j++;
		}

		delete [] positionInCompetition;
		// emptying simulationsList for next house (next round of simulations).
		simulationsList.clear();
		i++;
	}

	// print scoring function.

	//					algorithm, score per house,	trimmed_N, avg + spacers.
	const int rowLengthInChars = 13 + (numOfAlgorithms + 1) * 10 + (1 + numOfAlgorithms + 2) + 1;
	cout << setfill('-') << setw(rowLengthInChars) << '-' << setfill(' ') << endl; // dash-spacing line.
	cout << "|" << setw(13) << " " << "|"; // algorithms name empty column title.
	for (House& h : houses)
	{
		if (h.name.size() > 9)
		{
			h.name.resize(9); // if size is bigger then 9, shrinking it.
		}
		cout << setw(10) << left << h.name << "|";
	}
	cout << setw(10) << left << "AVG" << "|" << endl;

	int avgForAlgorithm;
	for (i = 0; i < numOfAlgorithms; i++)
	{
		cout << setfill('-') << setw(rowLengthInChars) << '-' << setfill(' ') << endl; // dash-spacing line.
		avgForAlgorithm = 0;
		// for each algorithm.
		j = 0;
		cout << "|" << setw(13) << left << "algo_name" + i << "|";
		for (House h : houses)
		{
			// for each house.

			cout << setw(10) << right << simulationScores[j][i]; // required for Targil 1 only.
			avgForAlgorithm += simulationScores[j][i];
			j++;
		}
		cout << "|" << setw(10) << right << "some n"; // trimmed-n
		cout << "|" << setw(10) << setprecision(3) << right << avgForAlgorithm/numOfHouses << "|" << endl;
		cout << setfill('-') << setw(rowLengthInChars) << '-' << setfill(' ') << endl; // dash-spacing line.
	}

	// freeing simlationSteps table.
	for (i = 0; i < numOfHouses; i++)
	{
		delete [] simulationSteps[i];
		delete [] simulationScores[i];
	}
	delete [] simulationSteps;
	delete [] simulationScores;
}