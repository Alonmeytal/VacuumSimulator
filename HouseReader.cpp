/*
 * username1: alonmeytal
 * ID1: 203972419
 *
 * username2:chenanily
 * ID2:305008864
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
