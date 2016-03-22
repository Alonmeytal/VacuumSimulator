/*
 * House.cpp
 *
 *  Created on: Mar 22, 2016
 *      Author: alonmeytal
 */

#include "House.h"
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

House::House(string fileName) {
	House h = House(); // creating new "empty" house using empty c'tor
	ifstream fin(fileName); // opening file-stream to ($fileName) containing the house
	getline(fin, h.name); // Reading house name
	getline(fin, h.description); // Reading house description
	fin >> h.rows; // Reading house rows
	fin >> h.cols; // Reading house cols
	fin.ignore(); // Skipping newline and going to the beginning of the matrix
	h.matrix = new char[h.rows][h.cols];
	string row;
	for (int i = 0; i < h.rows; i++)
	{
		getline(fin, row); //h.matrix[i]);
		for (int j = 0; j < h.cols; j++)
		{
			h.matrix[i][j] = row[j];
		}
	}
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

House::~House() {
	delete matrix;
}

