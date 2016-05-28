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
#include <functional>
#include <memory>

#include <dlfcn.h>
#include <stdlib.h> 
 
using namespace std;

// Interface(s) provided.
#include "AbstractAlgorithm.h"
#include "AlgorithmRegistrar.h"
// Actual Class(es)
#include "House.h"
// Function Objects
#include "Reader.h"
#include "Simulator.h"
#include "Errorton.h"

int defaultScore(const map<string,int>& score_params)
{
	try
	{
		int scoring_params = 2000 - ((score_params.at("actual_position_in_competition") - 1 ) * 50) +
			((score_params.at("winner_num_steps") - score_params.at("this_num_steps")) * 10) -
			((score_params.at("sum_dirt_in_house") - score_params.at("dirt_collected")) * 3) +
			((score_params.at("is_back_in_docking") == 1 ? 50 : -200));

		int score = max(0, scoring_params);
		return score;
	}
	catch(out_of_range)
	{
		// couldn't find one of the keys.
		return -1;
	}
}

int main(int argc, char ** argv) {
	Reader reader(argc, argv);
	Errorton& e = Errorton::getInstance();

	// Reading Settings from config.ini.
	map<string, int> settings = reader.getSettings();
	
	// errors were found, printing errors and exiting.
	if (e.reports() > 0)
	{
		return -1;
	}

	// Dynamically loading scoring function from score_formula.so.
	//std::function<int(const map<string,int>&)> score;
	int (*score)(const map<string,int>&);
	void * scoreDL = NULL;
	if (reader.getScorePath() == "-1")
	{
		// -score_formula was not provided, scoring fuction is the default formula.
		score = defaultScore;
	}
	else
	{
		// -score_formula was provided
		string scoringFilePath = reader.getScorePath() + "score_formula.so";
		// score formula full path
		char * fullPath = realpath(reader.getScorePath().c_str(), NULL);
		string fullPathStr(fullPath);
		free(fullPath);
		if (access(scoringFilePath.c_str(), F_OK) != 0)
		{
			// score_formula doesn't exist, printinf usage, error and exiting.
			e.reportError('u', reader.usageString);
			e.reportError('u', "cannot find score_formula.so file in '" + fullPathStr + "'");
			return -1;
		}
		// score_formula.so exists, trying to open it.
		scoreDL = dlopen(scoringFilePath.c_str(), RTLD_NOW);
		if (scoreDL == NULL)
		{
			// score_formula.so could not be loaded, printing error
			e.reportError('u', "score_formula.so exists in '" + fullPathStr + "' but cannot be opened or is not a valid .so");
			return -1;
		}
		else
		{
			score = (int (*)(const map<string,int>&)) dlsym(scoreDL, "calc_score");
			if (score == NULL)
			{
				// symbol couldn't be found.
				e.reportError('u',"score_formula.so exists in '" + fullPathStr + "' but cannot be opened or is not a valid .so");
				dlclose(scoreDL);
				return -1;
			}
			else
			{
				cout << "score_formula loaded!" << endl;
			}
		}
	}
	

	// get algorithms.
	//	get algorithms file list.
	char * fullPath = realpath(reader.getAlgorithmPath().c_str(), NULL);
	string fullAlgoPath(fullPath);
	free(fullPath);

	list<string> algorithmFilesList = reader.getAlgorithmFiles();
	if (algorithmFilesList.size() == 0)
	{
		// No algorithms were found, printing usage and exiting.
		e.reportError('u', reader.usageString);
		e.reportError('u', "cannot find algorithm files in '" + fullAlgoPath + "'");
		return -1;
	}
	//  load algorithms from files.
	AlgorithmRegistrar& regi = AlgorithmRegistrar::getInstance(); 
	int sizeBefore, sizeAfter;
	list<void *> dlList;

	for (string algorithmFile : algorithmFilesList)
	{
		string algorithmName = algorithmFile.substr(algorithmFile.find_last_of('/')+1);
		sizeBefore = regi.size();
	
		void * dlHandler = dlopen(algorithmFile.c_str(), RTLD_NOW); // trying to load .so file.
		if (dlHandler == NULL)
		{
			// dlHandler is empty because the file failed to load.
			e.reportError('a', algorithmName + ".so: file cannot be loaded or is not a valid .so");
			//cout << dlerror() << endl;
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
			e.reportError('a', algorithmName + ": valid .so but no algorithm was registered after loading it");
		}
		else
		{
			// Algorithm was added to AlgorithmRegistrar, adding it's name to the list.
			regi.setNameForLastAlgorithm(algorithmName.erase(algorithmName.find_last_of('.')));
		}
	}

	if (regi.size() == 0)
	{
		// No algorithms were registered.
		cout << "All algorithm files in target folder '" << fullAlgoPath << "' cannot be opened or are invalid:" << endl;
		return -1;
	}

	// get houses.
	//	get .house file list.
	fullPath = realpath(reader.getHousePath().c_str(),NULL);
	string fullHousePath(fullPath);
	free(fullPath);

	list<string> houseFilesList = reader.getHouseFiles();
	if (houseFilesList.size() == 0)
	{
		// No houses were found, printing usage and exiting.
		e.reportError('u', reader.usageString);
		e.reportError('u',"cannot find house files in '" + fullHousePath + "'");
		return -1;
	}
	//	create houses from files.
	list<House> houses;
	for (string houseFile : houseFilesList)
	{
		string houseName = houseFile.substr(houseFile.find_last_of('/')+1);
		try
		{
			houses.emplace_back(houseFile);	
		}
		catch (const exception& problem)
		{
			e.reportError('h', houseName + ": " + problem.what());
		}
	}
	houses.sort([](House a, House b) { return a.name < b.name; });
	if (houses.size() == 0)
	{
		// No houses were loaded properly.
		cout << "All house files in target folder '" << fullHousePath << "' cannot be opened or are invalid:" << endl;
		return -1;		
	}

	// run simulator on houses and algorithms
	Simulator(settings, houses, score, reader.getNumberOfThreads()).run();

	cout << "\nErrors:" << endl;

	// releasing scoring function point and .so
	if ((scoreDL != NULL) && (dlclose(scoreDL)))
	{
		return -1;
	}

	/*
	for (void * dlHandler : dlList)
	{
		if (dlclose(dlHandler))
		{
			return -1; // .so failed to close.
		}
	}
	*/
	return 0;
}
