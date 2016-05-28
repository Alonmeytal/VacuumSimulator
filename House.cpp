/*
 * username1: alonmeytal
 * ID1: 203972419
 *
 * username2:chenanily
 * ID2:305008864
 */

#include "House.h"
#include "Point.h"
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

House::House(string fileName) : House() {
	ifstream houseFileStream(fileName); // opening file-stream to ($fileName) containing the house
	string row;
	if (!houseFileStream.is_open())
	{
		throw runtime_error("cannot open file");
	}

	// Reading house name
	if(fileName.find_last_of('/') == 1)
	{
		name = fileName.substr(fileName.find_last_of('/')+1,fileName.find_last_of('.')-2);	
	}
	else
	{
		name = fileName.substr(fileName.find_last_of('/')+1,fileName.find_last_of('.')-fileName.find_last_of('/')-1);
	}
	getline(houseFileStream,row); // skipping name-description line.

	int tempNumber;
	// Reading house max Steps
	getline(houseFileStream,row);
	try
	{
		tempNumber = stoi(row);
		if (tempNumber > 0)
			maxSteps = tempNumber;
	}
	catch (invalid_argument)
	{
		throw runtime_error("line number 2 in house file shall be a positive number, found: " + row);
	}

	// Reading house rows
	getline(houseFileStream,row);
	try
	{
		tempNumber = stoi(row);
		if (tempNumber > 0)
			rows = tempNumber;
	}
	catch (invalid_argument)
	{
		throw runtime_error("line number 3 in house file shall be a positive number, found: " + row);
	}

	// Reading house columns
	getline(houseFileStream,row);
	try
	{
		tempNumber = stoi(row);
		if (tempNumber > 0)
			cols = tempNumber;
	}
	catch (invalid_argument)
	{
		throw runtime_error("line number 4 in house file shall be a positive number, found: " + row);
	}

	// reading House.matrix from file.
	int i, j;
	for (i = 0; i < rows; i++)
	{
		getline(houseFileStream, row); // reading current line from houseFileStream into $row
		if (houseFileStream.eof())
		{
			matrix.push_back(string(cols, 'W'));
		}
		else
		{
			// still within file house rows.
			row.resize(cols, 'W');
			matrix.push_back(row);
			for (j = 0; (j < cols) || (j < ((int) row.length())); j++)
			{
				if ((row[j] >= '1') && (row[j] <= '9'))
				{
					dirt += (row[j] - '0'); // cell contained dirt, accumulating it into House.dirt.
				}
				else if (row[j] == '0')
				{
					matrix[i][j] = ' '; // in case it accidently says '0', writing ' '(blank) instead.
				}

			}
		}
	}

	// rewriting boundaries into walls (in case they weren't)
	for (i = 0; i < cols; i++)
	{
		matrix[0][i] = 'W';
		matrix[rows-1][i] = 'W';
	}
	for (i = 0; i < rows; i++)
	{
		matrix[i][0] = 'W';
		matrix[i][cols-1] = 'W';
	}

	// counting docking points.
	int dockingCounter = 0;
	for (i = 1; i < rows; i++)
	{
		for (j = 1; j < cols; j++)
		{
			if (matrix[i][j] == 'D')
			{
				dockingPoint = Point(i,j);
				dockingCounter++;
			}
			else if((!isdigit(matrix[i][j])) && (matrix[i][j] != 'W') && (matrix[i][j] != ' '))
			{
				// character is not a valid character and thus should throw an exception
				matrix[i][j] = ' ';
			}
		}
	}
	if (dockingCounter == 0)
	{
		throw runtime_error("missing docking station (no D in house)");
	}
	if (dockingCounter > 1)
	{
		throw runtime_error("too many docking stations (more than one D in house)");
	}
	
	houseFileStream.close(); // releasing .house file resource.
}
House::House(const House & otherHouse) : House() {
	name = otherHouse.name;
	maxSteps = otherHouse.maxSteps;
	cols = otherHouse.cols;
	rows = otherHouse.rows;
	dirt = otherHouse.dirt;
	matrix = otherHouse.matrix;
	dockingPoint = Point(otherHouse.dockingPoint);
}

void House::print() {
	cout << "House name: " << name << endl; // Printing house's name
	// Printing house matrix
	for (int i = 0; i < rows; ++i)
	{
		for (int j =0; j < cols; ++j)
	    {
	      cout << matrix[i][j];
	    }
	    cout << endl;
	}
}

void House::vacuum(int x, int y){
	// If matrix[x][y] has dirt, decrease it (thank god number characters are consecutive).
	if (matrix[y][x] > '0' && matrix[y][x] <= '9')
		matrix[y][x]--;
}