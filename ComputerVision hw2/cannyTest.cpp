#include "cannyTest.h"
#include <cstdlib>
CanneyTest::CanneyTest()
{
	test_img = nullptr;
}

void CanneyTest::runtest()
{
	thres_limit_test();
	gaussian_test();
}

void CanneyTest::setTester(canny & tester)
{
	test_img =&tester;
}

void CanneyTest::run()
{
	test_img->cannyFilter();
}

void CanneyTest::grade(int n,vector<int> result)
{
	int points = 0;
	for (int i = 0; i < n; i++) {
		cin >> points;
		result.push_back(points);
	}
}



void CanneyTest::gaussian_test()
{
	int filter_size = 2;
	int sigma_size = 2;
	int init_filter = 3;
	double init_sigma = 1;
	int filter_add = 2;
	double sigma_add = 0.5;
	vector<CImgDisplay> disp;
	vector<int> filter_s;
	vector<double> sigma_s;
	for (int i = 0; i < filter_size; i++) {
		filter_s.push_back(init_filter + i*filter_add);

	}
	for (int i = 0; i < sigma_size; i++) {
		sigma_s.push_back(init_sigma + i*sigma_add);

	}


	for (int i = 0; i < filter_size; i++) {
		for (int j = 0; j < sigma_size; j++) {
			test_img->setFilter(filter_s[i], filter_s[i], sigma_s[j]);
			run();
			disp.push_back(test_img->return_final());
		}
	}
		for (int i = 0; i < filter_size*sigma_size; i++) {
			//string t = "id=" + i;
			//disp[i].set_title(t.data());
			disp[i].show();
		}
		grade(filter_size*sigma_size, g_result);
		system("pause");
	
}

void CanneyTest::thres_limit_test()
{
	int low_size = 2;
	int high_size = 2;
	int init_low =10;
	int init_high = 50;
	int low_add = 20;
	int high_add = 20;
	vector<CImgDisplay> disp;
	vector<int> thres_low;
	vector<int> thres_high;
	for (int i = 0; i < low_size; i++) {
		thres_low.push_back(init_low + i*low_add);

	}	
	for (int i = 0; i < high_size; i++) {
		thres_high.push_back(init_high + i*high_add);

	}
	

	for (int i = 0; i < low_size; i++) {
		for (int j = 0; j < high_size; j++) {
			test_img->setThreshold(thres_low[i], thres_high[j]);
			run();
			disp.push_back(test_img->return_final());
		}
	}
		for (int i = 0; i <low_size*high_size; i++) {
			//string t = "id=" + i;
			//disp[i].set_title(t.data());
			disp[i].show();
		}
		grade(low_size*high_size, t_result);
		system("pause");
	
}
