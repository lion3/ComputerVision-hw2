#include <cmath>
#include <iostream>
#include "canny.h"

using namespace std;
using namespace cimg_library;
canny::canny(string filepath_in)
{
	filepath = filepath_in;
	img = CImg<unsigned char>(filepath.data());

	if (!img.data()) // Check for invalid input
	{
		cout << "Could not open or find the image" << std::endl;
		filepath = "";

	}
}
void canny::cannyFilter() {
	
	if(filepath != "") {
		vector<vector<double>> filter = createFilter(gFilter_row, gFilter_col, gFilter_sigma);
		//Print filter
		for (int i = 0; i<filter.size(); i++)
		{
			for (int j = 0; j<filter[i].size(); j++)
			{
				cout << filter[i][j] << " ";
			}
		}
		grayscaled = toGrayScale(); // Grayscale the image
		gFiltered = useFilter(grayscaled, filter); // Gaussian Filter
		sFiltered = sobel(); // Sobel Filter

		non = nonMaxSupp(); // Non-Maxima Suppression
		thres = threshold(non, thres_low, thres_high); // Double Threshold and Finalize
		// show_img();
		
	}
}
void canny::setFilter(int row, int col, double sigma) {
	gFilter_row = row;
	gFilter_col = col;
	gFilter_sigma = sigma;
}
void canny::setThreshold(int low, int high)
{
	thres_low = low;
	thres_high = high;
}
CImg<unsigned char> canny::toGrayScale()
{
	grayscaled = CImg<unsigned char>(img.width(), img.height(), 1, 1);
	cimg_forXY(img, x, y) {
		int r = img(x, y, 0);
		int g = img(x, y, 1);
		int b = img(x, y, 2);
		double gray_value = (r * 0.2126 + g * 0.7152 + b * 0.0722);
		// cut off ?
		grayscaled(x, y, 0) = (unsigned char)gray_value;
	}
	return grayscaled;
}

vector<vector<double>> canny::createFilter(int row, int col, double sigma)
{
	// initialize filter with -1
	vector<vector<double>> filter;
	for (int i = 0; i < row; i++)
	{
		vector<double> temp_col;
		for (int j = 0; j < col; j++)
		{
			temp_col.push_back(-1);
		}
		filter.push_back(temp_col);
	}

	float coordSum = 0;
	float constant = 2.0 * sigma * sigma;
	float sum = 0.0; // sum for normalization

	// f(x,y) = (1/sqrt(Pi*2*sigma^2)) * exp(-(x^2+y^2)/2*sigma^2)
	for (int x = -row / 2; x <= row / 2; x++)
	{
		for (int y = -col / 2; y <= col / 2; y++)
		{
			coordSum = (x*x + y*y);
		//	filter[x + row / 2][y + col / 2] = (exp(-(coordSum) / constant)) / (M_PI * constant);
			filter[x + row / 2][y + col / 2] = exp(-(coordSum) / constant);
			sum += filter[x + row / 2][y + col / 2];
		}
	}
	// Normalize the Filter
	for (int i = 0; i < row; i++)
		for (int j = 0; j < col; j++)
			filter[i][j] /= sum;

	return filter;
}
CImg<unsigned char> canny::useFilter(CImg<unsigned char> img_in, vector<vector<double>> filterIn)
{
	int size = (int)filterIn.size() / 2;
	CImg<unsigned char> filteredImg = CImg<unsigned char>(img_in.height() - 2 * size, img_in.width() - 2 * size, 1,1);
	for (int i = size; i < img_in.height() - size; i++)
	{
		for (int j = size; j < img_in.width() - size; j++)
		{
			double sum = 0;
			// convolution
			for (int x = 0; x < filterIn.size(); x++)
				for (int y = 0; y < filterIn.size(); y++)
				{
					sum += filterIn[x][y] * (double)(img_in(i + x - size, j + y - size));
				}

			filteredImg(i - size, j - size) = (unsigned char)sum;
		}
	}
	return filteredImg;
}

CImg<unsigned char> canny::sobel()
{
	// Sobel X filter
	double x1[] = { -1.0,0,1.0 };
	double x2[] = { -2.0, 0, 2.0 };
	double x3[] = { -1.0, 0, 1.0 };

	vector<vector<double>> xFilter(3);
	xFilter[0].assign(x1, x1 + 3);
	xFilter[1].assign(x2, x2 + 3);
	xFilter[2].assign(x3, x3 + 3);

	// Sobel Y Filter
	double y1[] = { 1.0, 2.0, 1.0 };
	double y2[] = { 0, 0, 0 };
	double y3[] = { -1.0, -2.0, -1.0 };

	vector<vector<double>> yFilter(3);
	yFilter[0].assign(y1, y1 + 3);
	yFilter[1].assign(y2, y2 + 3);
	yFilter[2].assign(y3, y3 + 3);

	// Limit Size
	int size = (int)xFilter.size() / 2;

	CImg<unsigned char> filteredImg = CImg<unsigned char>(gFiltered.height() - 2 * size, gFiltered.width() - 2 * size, 1,1);

	angles = CImg<float>(gFiltered.height() - 2 * size, gFiltered.width() - 2 * size, 1, 1); //AngleMap

	for (int i = size; i < gFiltered.height() - size; i++)
	{
		for (int j = size; j < gFiltered.width() - size; j++)
		{
			double sumx = 0;
			double sumy = 0;

			for (int x = 0; x < xFilter.size(); x++)
				for (int y = 0; y < xFilter.size(); y++)
				{
					sumx += xFilter[x][y] * (double)(gFiltered(i + x - size, j + y - size)); //Sobel_X Filter Value
					sumy += yFilter[x][y] * (double)(gFiltered(i + x - size, j + y - size)); //Sobel_Y Filter Value
				}
			double sumxsq = sumx*sumx;
			double sumysq = sumy*sumy;

			double sq2 = sqrt(sumxsq + sumysq);

			if (sq2 > 255) //Unsigned Char Fix 强度图
				sq2 = 255;
			filteredImg(i - size, j - size) =(unsigned char)sq2;

			if (sumx == 0) //Arctan Fix 方向合成
				angles(i - size, j - size) = 90;
			else
				angles(i - size, j - size) = atan(sumy / sumx);
		}
	}

	return filteredImg;
}

CImg<unsigned char> canny::nonMaxSupp()
{
	CImg<unsigned char> nonMaxSupped = CImg<unsigned char>(sFiltered.height() - 2, sFiltered.width() - 2, 1, 1);
	for (int i = 1; i< sFiltered.height() - 1; i++) {
		for (int j = 1; j<sFiltered.width() - 1; j++) {
			float Tangent = angles(i, j);

			nonMaxSupped(i - 1, j - 1) = sFiltered(i, j);
			//Horizontal Edge
			if (((-22.5 < Tangent) && (Tangent <= 22.5)) || ((157.5 < Tangent) && (Tangent <= -157.5)))
			{
				if ((sFiltered(i, j) < sFiltered(i, j + 1)) || (sFiltered(i, j) < sFiltered(i, j - 1)))
					nonMaxSupped(i - 1, j - 1) = 0;
			}
			//Vertical Edge
			if (((-112.5 < Tangent) && (Tangent <= -67.5)) || ((67.5 < Tangent) && (Tangent <= 112.5)))
			{
				if ((sFiltered(i, j) < sFiltered(i + 1, j)) || (sFiltered(i, j) < sFiltered(i - 1, j)))
					nonMaxSupped(i - 1, j - 1) = 0;
			}

			//-45 Degree Edge
			if (((-67.5 < Tangent) && (Tangent <= -22.5)) || ((112.5 < Tangent) && (Tangent <= 157.5)))
			{
				if ((sFiltered(i, j) < sFiltered(i - 1, j + 1)) || (sFiltered(i, j) < sFiltered(i + 1, j - 1)))
					nonMaxSupped(i - 1, j - 1) = 0;
			}

			//45 Degree Edge
			if (((-157.5 < Tangent) && (Tangent <= -112.5)) || ((22.5 < Tangent) && (Tangent <= 67.5)))
			{
				if ((sFiltered(i, j) < sFiltered(i + 1, j + 1)) || (sFiltered(i, j) < sFiltered(i - 1, j - 1)))
					nonMaxSupped(i - 1, j - 1) = 0;
			}
		}
	}
	return nonMaxSupped;
}

CImg<unsigned char > canny::threshold(CImg<unsigned char> imgin, int low, int high)
{
	if (low > 255)
		low = 255;
	if (high > 255)
		high = 255;

	CImg<unsigned char > EdgeMat = CImg<unsigned char >(imgin.height(), imgin.width(), 1, 1);

	for (int i = 0; i < imgin.height(); i++)
	{
		for (int j = 0; j < imgin.width(); j++)
		{
			EdgeMat(i, j) = imgin(i, j);
			if (EdgeMat(i, j) > high)
				EdgeMat(i, j) = 255;
			else if (EdgeMat(i, j) < low)
				EdgeMat(i, j) = 0;
			else
			{
				bool anyHigh = false;
				bool anyBetween = false;
				for (int x = i - 1; x < i + 2; x++)
				{
					for (int y = j - 1; y < j + 2; y++)
					{
						if (x <= 0 || y <= 0 || EdgeMat.height() || y > EdgeMat.width()) //Out of bounds
							continue;
						else
						{
							if (EdgeMat(x, y) > high)
							{
								EdgeMat(i, j) = 255;
								anyHigh = true;
								break;
							}
							else if (EdgeMat(x, y) <= high && EdgeMat(x, y) >= low)
								anyBetween = true;
						}
					}
					if (anyHigh)
						break;
				}
				if (!anyHigh && anyBetween)
					for (int x = i - 2; x < i + 3; x++)
					{
						for (int y = j - 1; y < j + 3; y++)
						{
							if (x < 0 || y < 0 || x > EdgeMat.height() || y > EdgeMat.width()) //Out of bounds
								continue;
							else
							{
								if (EdgeMat(x, y) > high)
								{
									EdgeMat(i, j) = 255;
									anyHigh = true;
									break;
								}
							}
						}
						if (anyHigh)
							break;
					}
				if (!anyHigh)
					EdgeMat(i, j) = 0;
			}
		}
	}
	return EdgeMat;
}
CImgDisplay canny::return_final(){
	return CImgDisplay(thres,"",3U,false,true);
}
void canny::show_img()
{
	CImgDisplay img_disp(img, "img");
	CImgDisplay grayscaled_disp(grayscaled, "grayscaled");
	CImgDisplay sFiltered_disp(sFiltered, "sFiltered");
	CImgDisplay non_disp(non, "non");
	CImgDisplay thres_disp(thres, "thres");
	system("pause");
}
