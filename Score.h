/*
 * Score.h
 *
 *  Created on: Mar 23, 2016
 *      Author: alonmeytal
 */

#ifndef SCORE_H_
#define SCORE_H_

// thisNumSteps = simulationSteps;
// sumDirtInHouse = House.dirt;
int score(int positionInCompetition, int winnerNumSteps, int thisNumSteps, int sumDirtInHouse, int dirtCollected, bool isBackInDocking)
{
	return std::max(0, 2000 - ((positionInCompetition - 1) * 50) + ((winnerNumSteps - thisNumSteps) * 10)
			- ((sumDirtInHouse - dirtCollected) * 3) + (isBackInDocking ? 50 : -200));
}

#endif /* SCORE_H_ */
