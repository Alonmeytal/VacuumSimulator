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
	// get settings.
	map<string, string> settings = reader.getSettings();

	// get algorithms.
	//	get algorithms file list.
	list<string> algorithmFilesList = reader.getFiles(reader.algorithm_path, ".so");
	if ((algorthmFilesList.size() == 0) && (strAlgorithms.compare("./") != 0))
	{
		// no .so files were found in -algorithm_path and it wasn't the working directory.
		// trying to find .so files in working directory.
		algorithmFilesList = reader.getFiles("./", ".so");
		if ()
	}
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

	if (errorList.size() == 0)
	{
		// "(If there is a problem with algorithm files, we do not continue to check houses.)"
		errorList.push_front("All algorithm files in target folder '" + reader.algorithm_path + "'cannot be opened or are invalid:")
		return -1;
	}


	list<AbstractAlgorithm*> algorithms;
	for (pair<string, algorithm_constructor> algorithm : AlgorithmFactory)
	{
		algorithms.push_back(algorithm_constructor()); // maybe parenthesis are obsolete.
	}

	// get houses.
	//	get .house file list.
	list<string> houseFilesList = reader.getFiles(reader.house_path, ".house");
	//	create houses from files.
	list<House> houses;
	for (string HouseFile : housesFilesList)
	{
		houses.emplace_back(HouseFile);
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
