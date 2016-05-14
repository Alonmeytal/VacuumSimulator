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
#include <stdlib.h> 
#include <functional>
#include <memory>
 
using namespace std;

// Interface(s) provided.
#include "AbstractAlgorithm.h"
//#include "AlgorithmFactory.h"
#include "AlgorithmRegistrar.h"
// Actual Class(es)
#include "House.h"
// Function Objects
#include "Reader.h"
#include "Simulator.h"

//extern map<string, algorithm_constructor *> AlgorithmFactory;

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
		// No algorithms were found, printing usage and exiting.
		cout << reader.usageString << endl;
		//return 0;
	}
	//  load algorithms from files.
	AlgorithmRegistrar& regi = AlgorithmRegistrar::getInstance(); 
	int sizeBefore, sizeAfter;
	list<void *> dlList;

	for (string algorithmFile : algorithmFilesList)
	{
		sizeBefore = regi.size();
	
		void * dlHandler = dlopen(algorithmFile.c_str(), RTLD_NOW); // trying to load .so file.
		if (dlHandler == NULL)
		{
			// dlHandler is empty because the file failed to load.
			errorsList.push_back(algorithmFile + ": file cannot be loaded or is not a valid .so");
			cout << dlerror() << endl;
			continue;
		}
		else
		{
			dlList.push_back(dlHandler);
		}

		sizeAfter = regi.size();
		if (sizeAfter == sizeBefore)
		{
			// AlgorithmRegistrar didn't change and so no new algorithm was registered.
			errorsList.push_back(algorithmFile + ": valid .so but no algorithm was registered after loading it");
		}
		else
		{
			// Algorithm was added to AlgorithmRegistrar, adding it's name to the list.
			regi.setNameForLastAlgorithm(algorithmFile.substr(algorithmFile.find_last_of('/')+1,algorithmFile.find_last_of('.')-2));
		}
	}

	if (errorsList.size() > 0)
	{
		// "(If there is a problem with algorithm files, we do not continue to check houses.)"
		char * fullPath = realpath(reader.getAlgorithmPath().c_str(), NULL);
		string fullPathStr(fullPath);
		errorsList.push_front("All algorithm files in target folder '" + fullPathStr + "' cannot be opened or are invalid:");
		free(fullPath);
	}
	/*
	if (AlgorithmFactory.size() == 0)
	{
		// No algorithms were registered.
		printErrors(errorsList);
		return -1;
	}
	*/
	// get houses.
	//	get .house file list.
	list<string> houseFilesList = reader.getHouseFiles(errorsList);
	if (houseFilesList.size() == 0)
	{
		// No houses were found, printing usage and exiting.
		cout << reader.usageString << endl;
		return -1;
	}
	//	create houses from files.
	list<House> houses;
	errorsBefore = errorsList.size();
	for (string HouseFile : houseFilesList)
	{
		try
		{
			houses.emplace_back(HouseFile);	
		}
		catch (const exception& problem)
		{
			errorsList.push_back(HouseFile + ": " + problem.what());
		}
	}
	errorsAfter = errorsList.size();

	if(errorsAfter - errorsBefore > 0)
	{
		// errors occured during houses loading.
		char * fullPath = realpath(reader.getHousePath().c_str(),NULL);
		string fullPathStr(fullPath);
		errorsList.push_front("All house files in target folder '" + fullPathStr + "' cannot be opened or are invalid:");
		free(fullPath);
	}
	if (houses.size() == 0)
	{
		// No houses were loaded properly.
		printErrors(errorsList);
		return -1;		
	}

	// run simulator on houses and algorithms
	Simulator(settings, houses).run();
	
	for (void * dlHandler : dlList)
	{
		if (dlclose(dlHandler))
		{
			return -1; // .so failed to close.
		}
	}

	return 0;
}
