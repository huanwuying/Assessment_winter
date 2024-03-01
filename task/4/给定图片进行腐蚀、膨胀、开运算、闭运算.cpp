#include <opencv2/opencv.hpp>
#include<iostream>
#include<math.h>

using namespace cv;
using namespace std;

Mat src,dst;
char OUTPUT[] = "output image";
int element_size = 3;
int max_size = 21;

void CallBack_Demo(int, void*) {
	int s = element_size * 2 + 1;
	Mat kernel = getStructuringElement(MORPH_RECT, Size(s,s), Point(-1, -1));
	dilate(src, dst, kernel, Point(-1, -1), 1);  //膨胀
	erode(src,dst,kernel);  //腐蚀
	imshow(OUTPUT, dst);
	return;
}

int main(int argc, char** argv) {
	
	src = imread("D:/image/img1.jpg");
	
	if (!src.data) {
		printf("could load imgage....\n");
		return -1;
	}
	namedWindow("src", WINDOW_AUTOSIZE);
	imshow("src", src);
	
	namedWindow(OUTPUT,WINDOW_AUTOSIZE);
	//createTrackbar("Element Size:", OUTPUT_WIN, &element_size, max_size, CallBack_Demo);   //生成一个滑块
	//先创建Trackbar,在这里边先调用CallBackDemo,会在CallBackDemo上生成一个滑块，然后再调用执行CallBackDemo
	//CallBack_Demo(0, 0);
	
	Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3), Point(-1, -1));
	morphologyEx(src, dst, MORPH_OPEN, kernel);
	imshow(OUTPUT, dst);  


	waitKey(0);
	return 0;
}
