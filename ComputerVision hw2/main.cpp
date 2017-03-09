

#include <iostream>
#include "cannyTest.h"
#include <string>

using namespace std;

int main()
{
	string filePath = "C:\\Users\\xusy\\Downloads\\Ex2+-+Canny\\Ex2 - Canny\\test_Data\\lena.bmp"; // Filepath of input image
	canny cny(filePath);
	//cny.setThreshold(5, 40);
	//cny.cannyFilter();
	//vector<CImgDisplay> dispt;
	
	//CImgDisplay disp(cny.thres, "h1",3U,false,true);
	//dispt.push_back(disp);
	//cny.setThreshold(20, 80);
	//cny.cannyFilter();
	//CImgDisplay disp1(cny.thres, "h2",3U, false, true);
	//dispt.push_back(disp1);
	//cny.setThreshold(40, 80);
	//cny.cannyFilter();
	//CImgDisplay disp2(cny.thres, "h3", 3U, false, true);
	//dispt.push_back(disp2);
	//cny.setThreshold(10, 100);
	//cny.cannyFilter();
	//CImgDisplay disp3(cny.thres, "h4", 3U, false, true);
	//dispt.push_back(disp3);
	//for (int i = 0; i < 4; i++) {
	//	dispt[i].show();
	//}

	//cny.show_img();
//	cout << "here";
	CanneyTest cnyTest;
	cnyTest.setTester(cny);
	cnyTest.runtest();

	system("pause");
	return 0;
}
