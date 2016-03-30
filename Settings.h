/*
 * Settings.h
 *
 *  Created on: Mar 18, 2016
 *      Author: alonmeytal
 */

#ifndef SETTINGS_H_
#define SETTINGS_H_

#include <string>
#include <map>
#include <vector>

using namespace std;

void settingsFromFile(map<string,int>& set, string file);
string trim(string& str);
vector<string> split(const string &s, char delim);

#endif /* SETTINGS_H_ */
