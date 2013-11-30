/*
 * Solution.h
 */

#ifndef SOLUTION_H_
#define SOLUTION_H_

#include <map>
#include <assert.h>
#include "Instance.h"

class Solution {
public:
	Solution(Instance& inst);
	~Solution();
	bool addObject(int obj, int boxNumber);
	bool hasSpaceBox(int boxNumber, float weight);
	float amountSpaceInBox(int boxNumber);
	int createBox();

private:
	Instance* instance;
	std::map< int, std::vector<int>* > boxes;

};


#endif /* SOLUTION_H_ */