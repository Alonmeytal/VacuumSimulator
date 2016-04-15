/*
 * username1: alonmeytal
 * ID1: 203972419
 *
 * username2:chenanily
 * ID2:305008864
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include <map>
#include <string>
#include <vector>

#include <unistd.h>

using namespace std;

#include "House.h"
#include "AbstractAlgorithm.h"

class Reader {
	map<string,string> paths;

	const string usageString = "Usage: simulator [-config <config path>] [-house_path <house path>] [-algorithm_path <algorithm path>]";
	const string strConfig = "-config";
	const string strHouse = "-house_path";
	const string strAlgorithms = "-algorithm_path";

	Reader(const Reader& r) = delete; // copy c'tor
	Reader(const Reader&& r) = delete; // move c'tor

	void printErrors();
	list<string> getFilesFromPath(string path, string fileExtension);

	string trim(string& str);
	vector<string> split(const string &s, char delim);

public:
	Reader(int argc, char ** argv);

	int getSettings(list<string>& errorsList);
	int getHouseFiles(list<string>& errorsList);
	int getAlgorithmFiles(list<string>& errorsList);
};
