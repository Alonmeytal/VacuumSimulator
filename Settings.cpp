/*
 * Settings.cpp
 *
 *  Created on: Mar 18, 2016
 *      Author: alonmeytal
 */

#include "Settings.h"
#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

using namespace std;

void settingsFromFile(map<string,int>& set, string file)
{
    ifstream fin(file.c_str());
    string line;
    while (getline(fin, line))
    {
    	vector<string> tokens = split(line, '=');
    	if (tokens.size() != 2)
    	{
    		continue;
    	}
    	set[trim(tokens[0])] = atoi((char *) tokens[1]);
    }
}

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

