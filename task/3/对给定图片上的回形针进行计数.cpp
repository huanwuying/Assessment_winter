#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;

int main()
{
	cv::Mat srcMat = imread("C://Users//john//Desktop//1.jpg", 0);
	cv::Mat resMat;
	cv::Mat resover;
	cv::Mat resover_dec;
	cv::Mat resover_sign;
	cv::Mat Matstate;
	cv::Mat center;
	cv::Mat kernel(14, 14, CV_8UC1);
	//	imshow("src", srcMat);
	threshold(srcMat, resMat, 100, 255, THRESH_OTSU);  //大津法
	bitwise_not(resMat, resover);
	imshow("resover", resover);
//	morphologyEx(resover, resover_dec, 2, kernel, Point(0, 0), 1, BORDER_CONSTANT,morphologyDefaultBorderValue());//腐蚀
//	imshow("resover_dec", resover_dec);
	connectedComponentsWithStats(resover, resover_sign, Matstate, center, 8, CV_32S);  //连通域标记	
	int line = 0;
	int clipnum = 0;
	int i;
	line = Matstate.rows;
	for (i = 0; i < Matstate.rows; i++)
	{
		if (Matstate.at<int>(i, 4) >= 2000&& Matstate.at<int>(i, 4)<=8000)
		{

			clipnum = clipnum + 1;
			cv::Rect rect;
			rect.x = Matstate.at<int>(i, 0);
			rect.y = Matstate.at<int>(i, 1);
			rect.width = Matstate.at<int>(i, 2);
			rect.height = Matstate.at<int>(i, 3);
			rectangle(resover, rect, CV_RGB(255, 255, 255), 1, 8, 0);
		}

	}
	std::cout << clipnum << std::endl;
	imshow("resover", resover);
	waitKey(0);
}

