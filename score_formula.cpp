/*
 * username1: alonmeytal
 * ID1: 203972419
 *
 * username2:chenanily
 * ID2:305008864
 */
#ifndef SCORE_H_
#define SCORE_H_

#include <algorithm>
#include <map>
#include <string>
#include <stdexcept>

using namespace std;

extern "C" int calc_score(const map<string, int>& score_params)
{
	try
	{
		int scoring_params = 2000 - ((score_params.at("actual_position_in_competition") - 1 ) * 50) +
			((score_params.at("winner_num_steps") - score_params.at("this_num_steps")) * 10) -
			((score_params.at("sum_dirt_in_house") - score_params.at("dirt_collected")) * 3) +
			((score_params.at("is_back_in_docking") == 1 ? 50 : -200));

		int score = max(0, scoring_params);
		return score;
	}
	catch(out_of_range)
	{
		// couldn't find one of the keys.
		return -1;
	}
}

#endif /* SCORE_H_ */
