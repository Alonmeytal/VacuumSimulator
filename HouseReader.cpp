/*
 * HouseReader.cpp
 *
 *  Created on: Mar 24, 2016
 *      Author: alonmeytal
 */

#include "HouseReader.h"
#include <dirent.h>
#include <dir.h>

using namespace std;

void HouseReader::operator()(string housesPath){
	DIR * pdir;
	struct dirent *pent;
	pdir = opendir((char *) housesPath);
	if (pdir == NULL)
	{
		cout << "opendir failed" << endl;
		return;
	}
	while ((pent = readdir(pdir)) != NULL)
	{
		if (((string)pent->d_name).find(".house") != string::npos)
		{
			// file is a *.houes file.
			lst.push_back(House(housesPath + "/" + pent->d_name));
		}
	}
}
