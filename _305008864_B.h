#ifndef _305008864_B_H
#define _305008864_B_H
#include <array>
#include <stack>
#include <climits>
#include <memory>

#include "Direction.h"
#include "AbstractAlgorithm.h"
#include "AlgorithmRegistration.h"

using namespace std;

class _305008864_B: public AbstractAlgorithm
{
public:
	_305008864_B();

	virtual void setSensor(const AbstractSensor& sensor) { algoSensor = &sensor; };
	virtual void setConfiguration(map<string, int> config) { algoConfig = config; };
	//using the sensor's information returns the next move the simulated robot should do
	virtual Direction step(Direction prevStep);
	//alerts the algorithm that it must start finish the cleaning
	virtual void aboutToFinish(int stepsTillFinishing) { aboutToFinishClean = true; stepsTofinish = stepsTillFinishing; };
	virtual ~_305008864_B();


private:
	const AbstractSensor* algoSensor;
	map<string, int> algoConfig;
  bool operator<(const AbstractAlgorithm& otherAlgorithm)const;
	// an array of Directions, ordered by a some priority that the algo gives to the directions
	// when possible the algo returns the first direction in the array that doesn't leads to a wall
	array<Direction, 5> orderOfDirections;
	//a map containing directions as keys and booleans as values. A direction's value will be true in case it leads to a wall
	map<Direction, bool> isLeadsToWall;
	//stack containing the directions that the algo returned to the simulator in the past- following these directions will make the robot return to docking station
	stack<Direction> stepsHistory;
	//a bool which is initialized to be true, once the function aboutToFinish was called by the simulator
	bool aboutToFinishClean;
	//a integer containing the number of steps that the robot can do until he finished, initialized to MAX_INT and updated once function aboutToFinish was called by the simulator
	int stepsTofinish;
	//called once the robot can do new steps (and not coming back to docking station), returning the first Direction in  orderOfDirections array which doesn't lead to a wall
	Direction getNewDir();
	// updates isLeadsToWall map according to the algoSensor's SensorInformation which was returned to the algo when it called sense algoSensor's function
	void updateWallMap(const SensorInformation& currSensoInfo);
	// returns the opposite Direction of currDir
	Direction getOppositeDir(const Direction& currDir)const;


};

#endif //__305008864_B__H_
