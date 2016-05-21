/*
 * username1: alonmeytal
 * ID1: 203972419
 *
 * username2:chenanily
 * ID2:305008864
 */

using namespace std;

#include <dirent.h>
#include <cstring>

#include "Reader.h"


Reader::Reader(int argc, char ** argv) {
	paths[strConfig] = "./"; // -config argument default value.
	paths[strHouse] = "./"; // -house_path argument default value.
	paths[strAlgorithms] = "./"; // - algorithm_path default value
    paths[strScore] = "-1";

	if ((argc > 1) && (argc % 2))
	{
		// command-line arguments (one or more) were inserted, updating relevant values.
		for (int i = 1; i < ((argc % 2) ? argc : argc - 1); i += 2)
		{
			paths[argv[i]] = argv[i+1];
			if(paths[argv[i]][strlen(argv[i+1])-1] != '/')
			{
				// path did not end with trailing '/', adding it.
				paths[argv[i]].append("/");
			}

		}
	}
}

map<string,int> Reader::getSettings(list<string>& errorsList) {
	int argumentsRead = 0, badArgumentsRead = 0;
	map<string,int> settings;
	const string pathToConfig = paths[strConfig] + "config.ini";
	
	if (access(pathToConfig.c_str(), F_OK) != 0)
	{
		// file doesn't exist. usage should be printed and exit.
		errorsList.push_back(usageString);
		char * fullPath = realpath(paths[strConfig].c_str(), NULL);
		string fullPathStr(fullPath);
		errorsList.push_back("cannot find config.ini file in '" + fullPathStr + "'");
		free(fullPath);
		return settings;
	}

    ifstream configStream(pathToConfig);
    if (configStream.is_open() == false)
    {
    	// file couldn't be opened for reading.
    	errorsList.push_back("config.ini exists in '" + paths[strConfig] + "' but cannot be opened");
    	configStream.close();
    	return settings;
    }

    string line, missingLine, badLine;
  	list<string> missingArgumentsList {"MaxStepsAfterWinner", "BatteryCapacity", "BatteryConsumptionRate", "BatteryRechargeRate"};
  	list<string> badArgumentsList;
    // trying to read arguments
    while (getline(configStream, line))
    {
    	vector<string> tokens = split(line, '=');
    	if (tokens.size() != 2)
    	{
    		continue;
    	}
    	string settingKey = trim(tokens[0]);
    	try
    	{
    		// trying to convert string value to int. looking for bad values.
    		int tempSetting = std::stoi(tokens[1]);
    		if (tempSetting > 0)
    		{
    			settings[settingKey] = tempSetting;
    			missingArgumentsList.remove(settingKey); // removing found argument from missing's list.
    		}
    		else
    		{
    			badArgumentsRead++;
    			badArgumentsList.push_back(settingKey);
    		}	
    	}
    	catch (invalid_argument)
    	{
    		badArgumentsRead++;
			badArgumentsList.push_back(settingKey);
    	}
    	argumentsRead++;
    }

    // outputing missing parameters
    if (argumentsRead < 4)
    {
    	int missingArguments = 4 - argumentsRead;

    	missingLine = "config.ini missing " + to_string(missingArguments) + " parameter(s): ";
    	for (string missingSetting : missingArgumentsList)
    	{
 		   	missingLine += missingSetting + ", ";
    	}
    	errorsList.push_back(missingLine.erase(missingLine.find_last_of("' ") - 1));
    }
    // outputing bad valued parameters.
    if (badArgumentsRead > 0)
    {
    	badLine = "config.ini having bad values for " + to_string(badArgumentsRead) + " parameter(s): ";
    	
    	for (string badSetting : badArgumentsList)
    	{
    		badLine +=  badSetting + ", ";
    	}
    	errorsList.push_back(badLine.erase(badLine.find_last_of("' ") - 1));
    }
    configStream.close();
    return settings;
}

list<string> Reader::getHouseFiles(list<string>& errorsList) {
	list<string> housesFromPath = getFilesFromPath(paths[strHouse], ".house", errorsList);
	if ((paths[strHouse] != "./") && (housesFromPath.size() == 0))
	{
		// in-case path is empty (no houses), it is instructed to look for houses in the working directory.
		housesFromPath = getFilesFromPath("./", ".house", errorsList);
	}
	return housesFromPath;
}

list<string> Reader::getAlgorithmFiles(list<string>& errorsList) {
	list<string> algorithmsFromPath = getFilesFromPath(paths[strAlgorithms], ".so", errorsList);
	if ((paths[strAlgorithms] != "./") && (algorithmsFromPath.size() == 0))
	{
		// in-case path is empty (no algorithms), it is instructed to look for algorithms in the working directory.
		algorithmsFromPath = getFilesFromPath("./", ".so", errorsList);
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
		errorsList.push_back(usageString);
		return filePaths;
	}

	// adding all files with extension $fileExtension to filePaths.
	while ((fileEntity = readdir(pathDirectory)) != NULL)
	{
		if (((string)fileEntity->d_name).find(fileExtension) != string::npos)
		{
			filePaths.push_back(path + fileEntity->d_name);
		}
	}

	// returning list of all files found (might be empty).
	return filePaths;
}

int Reader::getNumberOfThreads()
{
    return 1;
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
