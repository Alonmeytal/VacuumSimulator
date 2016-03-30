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
	ifstream fin(fileName); // opening file-stream to ($fileName) containing the house
	getline(fin, name); // Reading house name
	getline(fin, description); // Reading house description
	fin >> rows; // Reading house rows
	fin >> cols; // Reading house col(um)s
	fin.ignore(); // Skipping newline and going to the beginning of the matrix
	matrix = new char*[rows];
	string row;
	for (int i = 0; i < rows; i++)
	{
		matrix[i] = new char[cols];
		getline(fin, row); //h.matrix[i]);
		for (int j = 0; j < cols; j++)
		{
		matrix[i][j] = row[j];
			if (row[j] == 'D')
			{
				dockingPoint = Point(i,j);
			}
			else if ((row[j] >= '1') && (row[j] <= '9'))
			{
				dirt += (row[j] - '0');
			}
		}
	}
}
House::House(const House & otherHouse) : House() {
	cols = otherHouse.cols;
	rows = otherHouse.rows;
	name = otherHouse.name;
	description = otherHouse.description;
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
	cout << "House description: " << description << endl; // Printing house's description
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

