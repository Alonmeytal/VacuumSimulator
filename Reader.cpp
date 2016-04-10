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

int Reader::readSettings(map<string,int>& settings) {
	int i = 0;
	const char * pathToConfig = paths[strConfig].c_str();
	if (access(pathToConfig, F_OK) != 0)
	{
		// file doesn't exist. usage should be printed and exit.
		return -1;
	}
    ifstream ConfigStream(pathToConfig, in);
    if (configStream.is_open() == false)
    {
    	// file couldn't be open for reading.
    	errorList.push_back("config.ini exist in '" + pathToConfig + "' but cannot be opened");
    	return -2;
    }
    string line;
    while (getline(fin, line))
    {
    	vector<string> tokens = split(line, '=');
    	if (tokens.size() != 2)
    	{
    		continue;
    	}
    	settings[trim(tokens[0])] = std::stoi(tokens[1]);
    	i++;
    }
    if (i == 4)
    {
    	return 0;
    }
    else
    {
    	line = "config.ini missing " + (4 - i) + " parameter(s):";
    	if (settings.count("MaxStepsAfterWinner") == 0)
    	{
    		line += (i++ < 4) ? " MaxStepsAfterWinner," : " MaxStepsAfterWinner," ";
    	}
    	if (settings.count("BatteryCapacity") == 0)
    	{
    		line += (i++ < 4 ) ? " BatteryCapacity," : " BatteryCapacity";
    	}
    	if (settings.count("BatteryConsumptionRate") == 0)
    	{
    		line += (i++ < 4 ) ? " BatteryConsumptionRate," : " BatteryConsumptionRate";
    	}
    	if (settings.count("BatteryRechargeRate") == 0)
    	{
    		line += " BatteryRechargeRate";
    	}
    	errorList.push_back(line);
    	return -2;
    }
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

// from recitation.
vector<string> split(const string &s, char delim) {
  vector<string> elems;
  stringstream ss(s);
  string item;
  while (getline(ss, item, delim)) {
      elems.push_back(item);
  }
  return elems;
}

string trim(string& str)
{
  str.erase(0, str.find_first_not_of(' '));       //prefixing spaces
  str.erase(str.find_last_not_of(' ')+1);         //surfixing spaces
  return str;
}
