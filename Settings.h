/*
 * username1: alonmeytal
 * ID1: 203972419
 *
 * username2:chenanily
 * ID2:305008864
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
