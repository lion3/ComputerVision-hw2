#pragma once
#ifndef _CANNYTEST_
#define _CANNYTEST__
#include <iostream>
#include "canny.h"
class CanneyTest
{
public:
	CanneyTest();
	void runtest();
	void setTester(canny& tester);
private:
	void run();
	void grade(int, vector<int>);
	void thres_limit_test();
	void gaussian_test();

	canny* test_img;
	vector<int> t_result;
	vector<int> g_result;
};


#endif // !_CANNYTEST_

