/*
 * username1: alonmeytal
 * ID1: 203972419
 *
 * username2:chenanily
 * ID2:305008864
 */

#include <string>
#include <list>
#include <vector>

#include <memory>
#include <thread>
#include <atomic>
#include <mutex>

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
	vector<House> houses;
	list<unique_ptr<AbstractAlgorithm>> algorithms;
	int (*score)(const map<string,int>&);

	map<string, map<string, int>> simulationScores;

	// Multi-Threading stuff.
	int numOfThreads;
	atomic_size_t nextHouse{0};
	mutex getHouse_Lock;

 	void simulatorThread();

public:
 	Simulator(map<string, int>& _settings, list<House>& _houses, int (*_score)(const map<string,int>&), int _threads) :
 	settings(_settings), houses(vector<House> {std::begin(_houses),std::end(_houses)}),
 	score(_score), numOfThreads(_threads) {};

	void run();
};