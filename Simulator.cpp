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
#include <vector>
#include <map>

#include <algorithm>
#include <cstring>

#include <memory>
#include <thread>
#include <atomic>
#include <mutex>

using namespace std;

#include "Simulator.h"
#include "Errorton.h"
#include "AlgorithmRegistrar.h"

void Simulator::simulatorThread() {
	House currentHouse;
	Errorton& e = Errorton::getInstance();
	AlgorithmRegistrar& regi = AlgorithmRegistrar::getInstance();

	while (nextHouse < houses.size())
	{
		{
			lock_guard<mutex> lock(getHouse_Lock); // locking for this innerScope only.
			currentHouse = houses[nextHouse++];
		}
		// got a new house to run sub-simulations on.
		
		// setting up simulation steps counting table for currentHouse.
		map<string, int> simulationSteps;
		for(auto algorithmName : regi.getAlgorithmNames())
		{
			simulationSteps[algorithmName] = 0;
		}

		// for each house;
		int stepsTaken, winnerSteps, stepsFromWinner, failedRobots, actualPlaceInCompetition, winnersThisRound;
		bool winnerFound, finishedThisRound;

		const list<string>& algorithmNames = regi.getAlgorithmNames();
		auto algorithmName = algorithmNames.begin();

		// ! run a simulation of the algorithm on the house
		list<Simulation> simulationsList;

		// for each algorithm;
		list<unique_ptr<AbstractAlgorithm>> algorithms = regi.getAlgorithms();
		int numOfAlgorithms = algorithms.size();

		for (auto& currentAlgorithm : algorithms)
		{
			// create a simulation instance that will run currentAlgorithm on currentHouse;
			simulationsList.emplace_back(std::move(currentAlgorithm), currentHouse, settings);
		}

		map<string, int> positionInCompetition;

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

			// making each algorithm make a single step.
			algorithmName = algorithmNames.begin();
			for (Simulation& sim : simulationsList)
			{
				// make sure currentAlgrotim didn't make a mistake and thus should make another step.
				if (simulationSteps[*algorithmName] != -1)
				{
					stepResult = sim.runStep();
					switch (stepResult)
					{
					case -1 :
						// algorithm made a mistake.
						simulationSteps[*algorithmName] = -1;
						e.reportError('a', "Algorithm " + *algorithmName + " when running on " + currentHouse.name + " went on a wall in step " + to_string(stepsTaken));
						failedRobots++;
						positionInCompetition[*algorithmName] = 10;
						break;
					case 1 :
						// algorithm finished.
						if (winnerFound == false)
						{
							// first winner found
							winnerFound = true; // flagging that a winner was found.
							winnerSteps = stepsTaken; // marking the winner's steps for scoring reference.
						}
						finishedThisRound = true; // flagging that at least one finished
						positionInCompetition[*algorithmName] = actualPlaceInCompetition; // saving the place in competition the algorithm got.
						winnersThisRound++; // counting the number of algorithms who finished this round.
						simulationSteps[*algorithmName] = stepsTaken; // remembering how many steps it took the algorithm to finish.
						break;
					default :
						// algorithm either returned 0 = nothing, or 2 = already done, didn't move.
						continue;
					}
				}
				algorithmName++;
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
		// if winner was not found, winnerSteps should be simulationSteps (stepsTaken).
		if (!winnerFound)
		{
			winnerSteps = stepsTaken;
		}

		// scoring the robots for currentHouse.
		algorithmName = algorithmNames.begin();
		map<string,int> score_params;
		for (Simulation& sim : simulationsList)
		{
			if (simulationSteps[*algorithmName] <= 0)
			{
				// robot did not finish
				simulationSteps[*algorithmName] = stepsTaken;
				positionInCompetition[*algorithmName] = 10;
				// setting score to 0 without calculating.
				simulationScores[currentHouse.name][*algorithmName] = 0;
				algorithmName++;
				continue;
			}
			score_params["actual_position_in_competition"] = ((positionInCompetition[*algorithmName] == 10) ? 10 : min(4,positionInCompetition[*algorithmName]));
			score_params["winner_num_steps"] = winnerSteps;
			score_params["this_num_steps"] = simulationSteps[*algorithmName];
			score_params["sum_dirt_in_house"] = currentHouse.dirt;
			score_params["dirt_collected"] = sim.dirtCollected;
			score_params["is_back_in_docking"] = sim.hasFinished;
			int simScore = score(score_params);
			simulationScores[currentHouse.name][*algorithmName] = simScore;
			simulationScores["AVG"][*algorithmName] += simScore;
			algorithmName++;
			// clearing score_params to make sure that no parameter is not being overwritten.
			score_params.clear();
		}
		// emptying simulationsList for next house (next round of simulations).
		simulationsList.clear();
	}
}


void Simulator::run() {
	// getting global singletons
	AlgorithmRegistrar& regi = AlgorithmRegistrar::getInstance();
	Errorton& e = Errorton::getInstance();

	// count houses and algorithms for future reference.
	int numOfHouses = houses.size();
	//int numOfAlgorithms = regi.size();

	// initatilizaing algorithm averages.
	const list<string>& algorithmNames = regi.getAlgorithmNames();
	for (auto algoName : algorithmNames)
	{
		simulationScores["AVG"][algoName] = 0;
	}

	// creating sub-simulator threads.
	vector<unique_ptr<thread>> threads((numOfHouses < numOfThreads) ? numOfHouses : numOfThreads);
	
	for (auto& thread_ptr : threads)
	{
		thread_ptr = make_unique<thread>(&Simulator::simulatorThread, this);
	}

	// waiting (joining) for all threads to finish
	for (auto& thread_ptr : threads)
	{
		thread_ptr->join();
	}

	// print scoring table.
	// titles row
	const int rowLengthInChars = 14 + ((numOfHouses + 1) * 11) + 2;
	cout << setfill('-') << setw(rowLengthInChars) << " " << setfill(' ') << endl; // dash-spacing line.
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
	cout << setfill('-') << setw(rowLengthInChars-1) << "-" << setfill(' ') << endl; // dash-spacing line.

	// algorithms' results;
	// calculating averages for every algorithm.
	list<pair<string, int>> algorithmAverages;
	for (auto algoName : algorithmNames)
	{
		simulationScores["AVG"][algoName] /= numOfHouses;
		algorithmAverages.emplace_back(algoName, simulationScores["AVG"][algoName]);
	}
	// sort algorithm names by average score for algorithm.
	algorithmAverages.sort([](pair<string,int> x, pair<string,int> y) { return y.second < x.second; });

	for (auto& algoPair : algorithmAverages)
	{
		// for each algorithm.
		string algoName = algoPair.first;
		cout << "|" << setw(13) << left << algoName << "|";
		for (House h : houses)
		{
			// for each house.
			if (simulationScores[h.name][algoName] == -1)
			{
				e.reportError('g',"Score formula could not calculate some scores, see -1 in the results table");
			}
			cout << setw(10) << right << simulationScores[h.name][algoName] << "|"; // required for Targil 1 only.
		}
		cout << setw(10) << setprecision(3) << right << simulationScores["AVG"][algoName] << "|" << endl;
		cout << setfill('-') << setw(rowLengthInChars) << " " << setfill(' ') << endl; // dash-spacing line
	}
}

/*
void Simulator::run() {
	// getting global singletons
	Errorton& e = Errorton::getInstance();
	AlgorithmRegistrar& regi = AlgorithmRegistrar::getInstance();

	int i, j;

	list<unique_ptr<AbstractAlgorithm>> algorithms;
	
	// count houses and algorithms for future reference.
	int numOfHouses = houses.size();
	int numOfAlgorithms = regi.size();

	// ! run a simulation of the algorithm on the house
	list<Simulation> simulationsList;

	// setting up simulation steps counting table : (house, algorithm) for scoring purposes.
	map <int, map <int, int>> simulationSteps;
	// also, setting up simulation scoring table.
	//map <int, map <int, int>> simulationScores; 

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
	const list<string>& algorithmNames = regi.getAlgorithmNames();
	auto algorithmName = algorithmNames.begin();
	map<string, map<string, int>> simulationScores;
	for (House& currentHouse : houses)
	{
		simulationScores[currentHouse.name]["AVG"] = 0;
		// for each algorithm;
		algorithms = regi.getAlgorithms();
		for (auto& currentAlgorithm : algorithms)
		{
			// create a simulation instance that will run currentAlgorithm on currentHouse;
			simulationsList.emplace_back(std::move(currentAlgorithm), currentHouse, settings);
		}

		vector<int> positionInCompetition(numOfAlgorithms,0);

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
			algorithmName = algorithmNames.begin();
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
						e.reportError('a', "Algorithm " + *algorithmName + " when running on " + currentHouse.name + " went on a wall in step " + to_string(stepsTaken));
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
				algorithmName++;
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
		algorithmName = algorithmNames.begin();
		map<string,int> score_params;
		for (Simulation& sim : simulationsList)
		{
			score_params["actual_position_in_competition"] = ((positionInCompetition[j] == 10) ? 10 : std::min(4,positionInCompetition[j]));
			score_params["winner_num_steps"] = winnerSteps;
			score_params["this_num_steps"] = simulationSteps[i][j];
			score_params["sum_dirt_in_house"] = currentHouse.dirt;
			score_params["dirt_collected"] = sim.dirtCollected;
			score_params["is_back_in_docking"] = sim.hasFinished;
			simulationScores[currentHouse.name][*algorithmName] = score(score_params);
			j++;
			algorithmName++;
		}

		// emptying simulationsList for next house (next round of simulations).
		simulationsList.clear();
		i++;
	}

	// print scoring function.
	// titles row
	const int rowLengthInChars = 14 + ((numOfHouses + 1) * 11) + 2;
	cout << setfill('-') << setw(rowLengthInChars) << " " << setfill(' ') << endl; // dash-spacing line.
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
	cout << setfill('-') << setw(rowLengthInChars-1) << "-" << setfill(' ') << endl; // dash-spacing line.

	// algorithms' results;
	// int avgForAlgorithm;
	// getting algorithm's names' iterator
	//const list<string>& algorithmNames = regi.getAlgorithmNames();
	algorithmName = algorithmNames.begin();

	for (i = 0; i < numOfAlgorithms; i++)
	{
		//avgForAlgorithm = 0;
		// for each algorithm.
		j = 0;
		cout << "|" << setw(13) << left << *algorithmName << "|";
		for (House h : houses)
		{
			// for each house.

			cout << setw(10) << right << simulationScores[h.name][*algorithmName] << "|"; // required for Targil 1 only.
			//avgForAlgorithm += simulationScores[j][i];
			j++;
		}
		cout << setw(10) << setprecision(3) << right << 200 << "|" << endl;
		cout << setfill('-') << setw(rowLengthInChars) << " " << setfill(' ') << endl; // dash-spacing line
		algorithmName++;
	}
}
*/