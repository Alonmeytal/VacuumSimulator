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
	list<string> errorList;
	map<string,string> paths;

	Reader(const Reader& r) = delete; // copy c'tor
	Reader(const Reader&& r) = delete; // move c'tor

	int readSettings(map<string,int>& settings);
	int readHouses(list<House>& houses);
	int readAlgorithms(list<AbstractAlgorithm*>& algorithms);

	void printErrors();
	list<string> getFilesFromPath(string path, string fileExtension);

	string trim(string& str);
	vector<string> split(const string &s, char delim);

public:
	Reader(int argc, char ** argv);
	int readFromPaths(map<string,int>& settings, list<House>& houses, list<AbstractAlgorithm*>& algorithms);
};
