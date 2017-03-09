#pragma once
#include <CImg.h>
#include <string>
#include <vector>
#ifndef _CANNY_
#define _CANNY_

#define M_PI 3.14159265358979323846

using namespace cimg_library;
using namespace std;
class canny
{
public:

	canny(string);
	void cannyFilter();
	void setFilter(int row, int col, double sigma);
	void setThreshold(int low,int high);
	void show_img();
	CImg<unsigned char> toGrayScale();
	vector<vector<double> > createFilter(int, int, double); // Creat a gaussian filter
	CImg<unsigned char> useFilter(CImg<unsigned char>, vector<vector<double> >);// Use specific filter
	CImg<unsigned char> sobel(); // Sobel filtering
	CImg<unsigned char> nonMaxSupp(); // Non-maxim supp
	CImg<unsigned char> threshold(CImg<unsigned char >, int, int); // Double threshold and finalize picture
	CImgDisplay return_final();

private:
	CImg<unsigned char> img; // Original Image
	CImg<unsigned char> grayscaled; // GrayScale
	CImg<unsigned char> gFiltered; // Gradient
	CImg<unsigned char> sFiltered; // Sobel Filtered
	CImg<float> angles; // Angle Map
	CImg<unsigned char> non; // Non-maxima suppress
	CImg<unsigned char> thres; // Double threshold and final

	int gFilter_row = 3;
	int gFilter_col = 3;
	double gFilter_sigma = 1.0;
	int thres_low = 20;
	int thres_high = 40;
	string filepath;

};


#endif // ! _CANNY_
