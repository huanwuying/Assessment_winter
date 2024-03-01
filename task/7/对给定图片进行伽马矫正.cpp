#include <opencv2\opencv.hpp>
using namespace cv;
 
int main()
{
	Mat img = imread("D:/Code/Image/half.jpg",0);
	imshow("原始图", img);
 
	Mat newImg = Mat::zeros(img.size(), img.type());
	for (int i = 0; i < img.rows; i++)
	{
		for (int j = 0; j < img.cols; j++)
		{
			newImg.at<uchar>(i, j) = pow(img.at<uchar>(i, j)/255.0, 0.3)*255.0;  //[0,1]才会有gamma特性,0.2^0.3 == 0.6
		}
	}
 
	imshow("效果图", newImg);
	waitKey(0);
	return 0;
}