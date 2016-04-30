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
#include <dlfcn.h>
using namespace std;

// Interface(s) provided.
#include "Direction.h"
#include "SensorInformation.h"
#include "AbstractSensor.h"
#include "AbstractAlgorithm.h"
#include "AlgorithmFactory.h"
// Actual Class(es)
#include "House.h"
#include "Simulation.h"
// Function Objects
#include "Reader.h"
#include "Simulator.h"

extern map<string, algorithm_constructor> AlgorithmFactory;

int main(int argc, char ** argv)
{
	Reader reader(argc, argv);
	list<string> errorsList;
	int errorsBefore, errorsAfter;
	// get settings.
	errorsBefore = errorsList.size();
	map<string, int> settings = reader.getSettings(errorsList);
	errorsAfter = errorsList.size();
	if (errorsAfter - errorsBefore > 0)
	{
		printErrors(errorsList);
		return -1;
	}

	// get algorithms.
	//	get algorithms file list.
	list<string> algorithmFilesList = reader.getAlgorithmFiles(errorsList);

	//  load algorithms from files.
	int sizeBefore, sizeAfter;
	list<void *> dlList;

	for (string algorithmFile : algorithmFilesList)
	{
		sizeBefore = AlgorithmFactory.size();
		void * dlHandler = dlopen(algorithmFile, RTLD_NOW); // trying to load .so file.
		if (dlHandler == null)
		{
			// dlHandler is empty because the file failed to load.
			errorsList.push_back(algorithmFile + ": file cannot be loaded or is not a valid .so")
		}
		else
		{
			dlList.push_back(dlHandler);
		}
		sizeAfter = AlgorithmFactory.size();
		if (sizeAfter == sizeBefore)
		{
			// AlgorithmFactory didn't change and so no new algorithm was registered.
			errorsList.push_back(algorithmFile + ": valid .so but no algorithm was registered after loading it");
		}
	}

	if (errorsList.size() > 0)
	{
		// "(If there is a problem with algorithm files, we do not continue to check houses.)"
		errorsList.push_front("All algorithm files in target folder '" + reader.getAlgorithmPath() + "'cannot be opened or are invalid:")
		printErrors(errorsList);
		return -1;
	}


	list<AbstractAlgorithm*> algorithms;
	for (pair<string, algorithm_constructor> algorithm : AlgorithmFactory)
	{
		algorithms.push_back(algorithm_constructor()); // maybe parenthesis are obsolete.
	}

	// get houses.
	//	get .house file list.
	list<string> houseFilesList = reader.getHouseFiles(errorsList);
	//	create houses from files.
	list<House> houses;
	errorsBefore = errorsList.size();
	for (string HouseFile : housesFilesList)
	{
		try
		{
			houses.emplace_back(HouseFile);	
		}
		catch (const char * problem)
		{
			errorsList.push_back(HouseFile + ": " + problem);
		}
	}
	errorsAfter = errorsList.size();

	if(errorsAfter - errorsBefore > 0)
	{
		// errors eccured during houses loading.
		errrosList.push_front("All house files in target folder '" + reader.getHousePath() + "' cannot be opened or are invalid:")
	}

	// run simulator on houses and algorithms
	Simulator(houses, algorithms).run();

	for (void * dlHandler : dlList)
	{
		if (dlclose(dlHandler))
		{
			return -1; // .so failed to close.
		}
	}

	return 0;
}
