/*
 * username1: alonmeytal
 * ID1: 203972419
 *
 * username2:chenanily
 * ID2:305008864
 */

using namespace std;

#include <dir.h>
#include <dirent.h>

#include "Reader.h"


Reader::Reader(int argc, char ** argv) {
	paths[strConfig] = "./"; // -config argument default value.
	paths[strHouse] = "./"; // -house_path argument default value.
	paths[strAlgorithms] = "./"; // - algorithm_path default value

	if ((argc > 1) && (argc % 2))
	{
		// command-line arguments (one or more) were inserted, updating relevant values.
		for (int i = 1; i < ((argc % 2) ? argc : argc - 1); i += 2)
		{
			paths[argv[i]] = argv[i+1];
		}
	}
}

map<string,int> Reader::getSettings(list<string>& errorsList) {
	int i = 0;
	const char * pathToConfig = paths[strConfig].c_str();
	if (access(pathToConfig, F_OK) != 0)
	{
		// file doesn't exist. usage should be printed and exit.
		errorsList.push_back(usageString);
		return settings;
	}
    ifstream ConfigStream(pathToConfig, in);
    if (configStream.is_open() == false)
    {
    	// file couldn't be open for reading.
    	errorsList.push_back("config.ini exist in '" + pathToConfig + "' but cannot be opened");
    	return settings;
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

    if (i < 4)
    {
    	line = "config.ini missing " + (4 - i) + " parameter(s):";
    	if (settings.count("MaxStepsAfterWinner") == 0)
    	{
    		line += (i++ < 4) ? " MaxStepsAfterWinner," : " MaxStepsAfterWinner";
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
    	errorsList.push_back(line);
    }
    return settings;
}

list<string> Reader::getHouseFiles(list<string>& errorsList) {
	list<string> housesFromPath = getFilesFromPath(paths[strHouse], ".house", errorsList);
	if ((path[strAlgorithms] != "./") && (alogirthmsFromPath.size() == 0))
	{
		// in-case path is empty (no houses), it is instructed to look for houses in the working directory.
		housesFromPath = getFilesFromPath("./", ".house", errorsList);
	}
	return housesFromPath;
}

list<string> Reader::getAlgorithmsFiles(list<string>& errorsList) {
	list<string> algorithmsFromPath = getFilesFromPath(paths[strAlgorithms], ".so", errorsList);
	if ((paths[strAlgorithms] != "./") && (algorithmsFromPath.size() == 0))
	{
		// in-case path is empty (no algorithms), it is instructed to look for algorithms in the working directory.
		algorithmsFromPath = getFilesFrompath("./", ".so", errorsList);
	}
	return algorithmsFromPath;
}

list<string> Reader::getFilesFromPath(string path, string fileExtension, list<string>& errorsList) {
	list<string> filePaths;

	DIR * pathDirectory;
	struct dirent * fileEntity;

	// Opening directory at path.
	if ((pathDirectory = opendir(path.c_str())) == NULL)
	{
		// Directory couldn't be opened, returning usage.
		errorsList.push_back(UsageString);
		return filePaths;
	}

	// adding all files with extension $fileExtension to filePaths.
	while ((fileEntity = readdir(pathDirectory)) != NULL)
	{
		if (((string)fileEntity->d_name).find(fileExtension) != string::npos)
		{
			filePaths.push_back(housesPath + "/" + fileEntity->d_name));
		}
	}

	// returning list of all files found (might be empty).
	return filePaths;
}

// from recitation.
vector<string> Reader::split(const string &s, char delim) {
  vector<string> elems;
  stringstream ss(s);
  string item;
  while (getline(ss, item, delim)) {
      elems.push_back(item);
  }
  return elems;
}

string Reader::trim(string& str)
{
  str.erase(0, str.find_first_not_of(' '));       //prefixing spaces
  str.erase(str.find_last_not_of(' ')+1);         //surfixing spaces
  return str;
}
