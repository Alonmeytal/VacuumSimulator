/*
 * Settings.h
 *
 *  Created on: Mar 18, 2016
 *      Author: alonmeytal
 */

#ifndef SETTINGS_H_
#define SETTINGS_H_

#include <string>
#include <vector>
using namespace std;

typedef struct _Settings
{
	int maxSteps;
	int maxStepsAfterWinner;
	int batteryCapacity;
	int batteryConsumptionRate;
	int batteryRechargeRate;
} Settings;

void settingsFromFile(Settings& set, string file);
string trim(string& str);
vector<string> split(const string &s, char delim);

#endif /* SETTINGS_H_ */
