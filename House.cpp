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
	if (!houseFileStream.is_open())
	{
		throw "cannot open file";
	}
	getline(houseFileStream, name); // Reading house name
	houseFileStream >> maxSteps; // Reading house max Steps
	houseFileStream >> rows; // Reading house rows
	houseFileStream >> cols; // Reading house columns

	// reading House.matrix from file.
	matrix = new char*[rows]; // assigning rows of char[], according to House.rows
	string row;

	int i, j;
	for (i = 0; i < rows; i++)
	{
		getline(houseFileStream, row); // reading current line from houseFileStream into $row
		matrix[i] = new char[cols]; // assigning row to memory, according to House.cols
		if (houseFileStream.eof())
		{
			// house file was too short (didn't have enough rows).
			for (j = 0; j < cols; j++)
			{
				matrix[i][j] = 'W'; // filling remaining rows with walls.
			}
		}
		else
		{
			// still within file house rows.
			for (j = 0; (j < cols) || (j < ((int) row.length())); j++)
			{
				matrix[i][j] = row[j]; // copying value into house matrix.
				if ((row[j] >= '1') && (row[j] <= '9'))
				{
					dirt += (row[j] - '0'); // cell contained dirt, accumulating it into House.dirt.
				}
				else if (row[j] == '0')
				{
					matrix[i][j] = ' '; // in case it accidently says '0', writing ' '(blank) instead.
				}

			}
			if (((int) row.length()) < cols)
			{
				// row was shorter then needed, completing with walls.
				for (j = row.length(); j < cols; j++)
				{
					matrix[i][j] = 'W';
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
			else if (((matrix[i][j] < '1') && (matrix[i][j] > '9')) && ((matrix[i][j] != 'W') && (matrix[i][j] != ' ')))
			{
				// character is not a valid character and thus should throw an exception
				char * problem;
				sprintf(problem, "line number %d in house file shall be a positive number, found: %c", i, matrix[i][j]);
				// = "line number " + i + " in house file shall be a positive number, found: " + matrix[i][j];
				throw problem;
			}
		}
	}
	if (dockingCounter == 0)
	{
		throw "missing docking station (no D in house)";
	}
	if (dockingCounter > 1)
	{
		throw "too many docking stations (more than one D in house)";
	}
}
House::House(const House & otherHouse) : House() {
	name = otherHouse.name;
	maxSteps = otherHouse.maxSteps;
	cols = otherHouse.cols;
	rows = otherHouse.rows;

	// assigning and copying matrix. char-by-char \O.O/
	matrix = new char*[rows];
	for (int i = 0; i < rows; i++)
	{
		matrix[i] = new char[cols];
		for (int j = 0; j < cols; j++)
		{
			matrix[i][j] = otherHouse.matrix[i][j];
		}
	}
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

bool House::isValid() {
	// rewriting boundaries into walls.
	int i, j;
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

	// searching for one and only one docking point
	// going over the inside only (since boundaries have been fixed)
	int dockCount = 0;
	for (i = 1; i < rows - 1; i++)
	{
		for (j = 1; j < cols - 1; j++)
		{
			if (matrix[i][j] == 'D')
			{
				dockCount++;
			}
			else if ((matrix[i][j] <= '0' || matrix[i][j] > '9') && (matrix[i][j] != 'W'))
			{
				// cell isn't a docking point(D), a dirt level (0-9) or a wall (W)
				matrix[i][j] = ' ';
			}
		}
	}
	if (dockCount > 1)
		return false;
	return true;
}
House::~House() {
	delete matrix;
}

