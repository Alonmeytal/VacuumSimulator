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
#include <memory>

using namespace std;

// Interface(s) provided.
#include "AbstractSensor.h"
#include "AbstractAlgorithm.h"
#include "AlgorithmRegistrar.h"
// Actual Class(es)
#include "House.h"
#include "Simulation.h"

class Simulator {
	map<string,int> settings;
	list<House> houses;
	list<unique_ptr<AbstractAlgorithm>> algorithms;

	int score(int positionInCompetition, int winnerNumSteps, int thisNumSteps, int sumDirtInHouse, int dirtCollected, bool isBackInDocking)
	{
		return std::max(0, 2000 - ((positionInCompetition - 1) * 50) + ((winnerNumSteps - thisNumSteps) * 10)
			- ((sumDirtInHouse - dirtCollected) * 3) + (isBackInDocking ? 50 : -200));
	}

public:
 	Simulator(map<string, int>& _settings, list<House>& _houses) : settings(_settings), houses(_houses) {};
	void run();
};