/*
 * username1: alonmeytal
 * ID1: 203972419
 *
 * username2:chenanily
 * ID2:305008864
 */

using namespace std;

#include "Reader.h"

Reader::Reader(int argc, char ** argv) {
	const string strConfig = "-config";
	const string strHouse = "-house_path";
	const string strAlgorithms = "-algorithm_path";

	paths[strConfig] = "./"; // -config argument default value.
	paths[strHouse] = "./"; // -house_path argument default value.
	paths[strAlgorithms] = "./"; // - algorithm_path default value

	if (argc > 1)
	{
		// command-line arguments (one or more) were inserted, updating relevant values.
		for (int i = 1; i < argc; i += 2)
		{
			paths[argv[i]] = argv[i+1];
		}
	}
}

int Reader::readFromPaths(map<string,int>& settings, list<House>& houses, list<AbstractAlgorithm*>& algorithms) {
	// if reading failed completely return -1 : print usage & exit.
	// if reading failed partly return -2 : print errorsList (internal list<string> containing errors) & exit.
	// otherwise continue.
	const string usageString = "Usage: simulator [-config <config path>] [-house_path <house path>] [-algorithm_path <algorithm path>]";
	int settingsValue = readSettings(settings); // read settings.
	if (settingsValue == -1)
	{
		cout << usageString << endl;
		return -1;
	}
	else if (settingsValue == -2)
	{
		printErrors();
		return -2;
	}
	// settings had no probelms, moving on to algorithms.
	int algorithmsValue = readAlgorithms(algorithms);
	if (algorithmsValue == -1)
	{
		cout << usageString << endl;
		return -1;
	}
	else if(algorithmsValue == -2)
	{
		printErrors();
		return -2;
	}
	// everything ok with algorithms, continuing to houses.
	int housesValue = readHouses(houses);
	if (housesValue == -1)
	{
		cout << usageString << endl;
		return -1;
	}
	else if (housesValue == -2)
	{
		printErrors();
		return -2;
	}
	return 0;
}

int Reader::readSettings(map<string,int>& settings){
	return 0;
}

int Reader::readHouses(list<House>& houses) {
	return 0;
}

int Reader::readAlgorithms(list<AbstractAlgorithm*>& algorithms) {
	return 0;
}

list<string> getFilesFromPath(string path, string fileExtension) {
	list<string> filePaths;



	return filePaths;
}

void printErrors() {

}
