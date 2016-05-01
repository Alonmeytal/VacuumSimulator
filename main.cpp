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
#include "AbstractAlgorithm.h"
#include "AlgorithmFactory.h"
// Actual Class(es)
#include "House.h"
// Function Objects
#include "Reader.h"
#include "Simulator.h"

extern map<string, algorithm_constructor *> AlgorithmFactory;

void printErrors(list<string>& errorsList) {
	for (string e : errorsList)
	{
		cout << e << endl;
	}
}

int main(int argc, char ** argv) {
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
	if (algorithmFilesList.size() == 0)
	{
		cout << "No algorithms found." << endl;
		//return 0;
	}
	//  load algorithms from files.
	int sizeBefore, sizeAfter;
	list<void *> dlList;

	for (string algorithmFile : algorithmFilesList)
	{
		sizeBefore = AlgorithmFactory.size();
		void * dlHandler = dlopen(algorithmFile.c_str(), RTLD_NOW); // trying to load .so file.
		if (dlHandler == NULL)
		{
			// dlHandler is empty because the file failed to load.
			errorsList.push_back(algorithmFile + ": file cannot be loaded or is not a valid .so");
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
		errorsList.push_front("All algorithm files in target folder '" + reader.getAlgorithmPath() + "'cannot be opened or are invalid:");
		printErrors(errorsList);
		return -1;
	}


	list<AbstractAlgorithm*> algorithms;
	for (auto algorithm : AlgorithmFactory)
	{
		algorithms.push_back(algorithm.second()); //algorithm_constructor); // maybe parenthesis are obsolete.
	}

	// get houses.
	//	get .house file list.
	list<string> houseFilesList = reader.getHouseFiles(errorsList);
	//	create houses from files.
	list<House> houses;
	errorsBefore = errorsList.size();
	for (string HouseFile : houseFilesList)
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
		errorsList.push_front("All house files in target folder '" + reader.getHousePath() + "' cannot be opened or are invalid:");
		printErrors(errorsList);
	}

	// run simulator on houses and algorithms
	Simulator(settings, houses, algorithms).run();

	for (void * dlHandler : dlList)
	{
		if (dlclose(dlHandler))
		{
			return -1; // .so failed to close.
		}
	}

	return 0;
}
