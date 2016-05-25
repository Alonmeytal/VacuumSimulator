#include "_305008864_C.h"

_305008864_C::_305008864_C(){
	stepsTofinish = INT_MAX;
	aboutToFinishClean = false;
	//current difference between algos:
	orderOfDirections = { Direction::West, Direction::North, Direction::East, Direction::South, Direction::Stay };
	for (auto itr = orderOfDirections.begin(); itr != orderOfDirections.end(); itr++) {
		isLeadsToWall[*itr] = false;
	}
}


Direction _305008864_C::step(Direction prevStep) {
	Direction nextDir;
	if(aboutToFinishClean ==false || (stepsTofinish > (int)stepsHistory.size()+1)){
		SensorInformation currPosStatus = algoSensor->sense();
		updateWallMap(currPosStatus);
		nextDir = getNewDir();
		if (nextDir != Direction::Stay) {
			stepsHistory.push(getOppositeDir(nextDir));
		}
	}
	//need to return to docking. based on history
	else {
    if (stepsHistory.empty()==false){
  		nextDir = stepsHistory.top();
  		stepsHistory.pop();
   }
   else{
     nextDir=Direction::Stay;
   }
	}
	if (aboutToFinishClean == true) {
		stepsTofinish--;
	}

	return nextDir;
}


bool _305008864_C::operator<(const AbstractAlgorithm& otherAlgorithm)const
{
	return false;
}

void _305008864_C::updateWallMap(const SensorInformation& currSensoInfo) {
	int len = sizeof(currSensoInfo.isWall) / sizeof(bool);
	array<Direction, 4> dirArr = { Direction::East, Direction::West, Direction::South, Direction::North };
	for (int i = 0; i < len; i++) {
		isLeadsToWall[dirArr[i]] = currSensoInfo.isWall[i];
	}
}

Direction _305008864_C::getNewDir() {
	for (auto itr = orderOfDirections.begin(); itr != orderOfDirections.end(); itr++)
	{
		if (isLeadsToWall[*itr] == false){
			return *itr;
		}
	}
	return Direction::Stay;
}

Direction _305008864_C::getOppositeDir(const Direction& currDir)const {
	switch (currDir) {
	case (Direction::North) :
		return Direction::South;
	case (Direction::South) :
		return Direction::North;
	case (Direction::East) :
		return Direction::West;
	case (Direction::West) :
		return Direction::East;
	default:
		return Direction::Stay;
	}
}

_305008864_C::~_305008864_C() { }

REGISTER_ALGORITHM (_305008864_C)