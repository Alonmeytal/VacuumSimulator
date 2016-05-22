#include <iostream>
#include <list>

#include "Errorton.h"

using namespace std;

void Errorton::reportError(char c, const string& s) {
	if (c == 'u')
	{
		usageErrors.push_back(s);
	}
	else if (c == 'h')
	{
		houseErrors.push_back(s);
	}
	else if (c == 'a')
	{
		algorithmErrors.push_back(s);
	}
	else
	{
		generalErrors.push_back(s);
	}
	reportsCounter++;
}

Errorton::~Errorton() {
	if (usageErrors.size() > 0)
	{
		for (auto& s : usageErrors)
		{
			cout << s << endl;
		}
	}
	else
	{
		// printing hosue loading errors.
		for (auto& s : houseErrors)
		{
			cout << s << endl;
		}
		// printing algorithm loading and running errors.
		for (auto& s : algorithmErrors)
		{
			cout << s << endl;
		}
		// printing general probelm, such as scoring alert.
		for (auto& s : generalErrors)
		{
			cout << s << endl;
		}
	}
}


Errorton Errorton::instance;