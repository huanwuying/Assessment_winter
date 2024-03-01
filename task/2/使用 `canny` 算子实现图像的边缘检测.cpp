#include <opencv2/opencv.hpp>
#include <math.h>
 
#define _USE_MATH_DEFINES
 
using namespace cv;// 使用命名空间cv
 
 
/**
 将两个图像拼接，以便在同一个窗口显示
 dst 输出的拼接后的图像
 src1 拼接的第一幅图
 src2 拼接的第二幅图
 */
void mergeImg(Mat & dst,Mat &src1,Mat &src2) {
    int rows = src1.rows;
    int cols = src1.cols+5+src2.cols;
    CV_Assert(src1.type () == src2.type ());
    dst.create (rows,cols,src1.type ());
    src1.copyTo (dst(Rect(0,0,src1.cols,src1.rows)));
    src2.copyTo (dst(Rect(src1.cols+5,0,src2.cols,src2.rows)));
}
 
 
/**
 一维高斯卷积，对每行进行高斯卷积
 img 输入原图像
 dst  一维高斯卷积后的输出图像
 */
void gaussianConvolution(Mat &img, Mat &dst) {
    int nr = img.rows;
    int nc = img.cols;
    int templates[3] = {1, 2, 1};
    
    // 按行遍历除每行边缘点的所有点
    for (int j = 0; j < nr; j++) {
        uchar* data= img.ptr<uchar>(j); //提取该行地址
        for (int i = 1; i < nc-1; i++) {
            int sum = 0;
            for (int n = 0; n < 3; n++) {
                sum += data[i-1+n] * templates[n]; //相称累加
            }
            sum /= 4;
            dst.ptr<uchar>(j)[i] = sum;
        }
    }
}
 
 
/**
 高斯滤波器，利用3*3的高斯模版进行高斯卷积
 img 输入原图像
 dst  高斯滤波后的输出图像
*/
void gaussianFilter(Mat &img, Mat &dst) {
    // 对水平方向进行滤波
    Mat dst1 = img.clone();
    gaussianConvolution(img, dst1);
    //图像矩阵转置
    Mat dst2;
    transpose(dst1, dst2);
    // 对垂直方向进行滤波
    Mat dst3 = dst2.clone();
    gaussianConvolution(dst2, dst3);
    // 再次转置
    transpose(dst3, dst);
}
 
 
/**
 用一阶偏导有限差分计算梯度幅值和方向
 img 输入原图像
 gradXY 输出的梯度幅值
 theta 输出的梯度方向
 */
void getGrandient (Mat &img, Mat &gradXY, Mat &theta) {
    gradXY = Mat::zeros(img.size(), CV_8U);
    theta = Mat::zeros(img.size(), CV_8U);
    
    for (int j = 1; j < img.rows-1; j++) {
        for (int i = 1; i < img.cols-1; i++) {
            double gradY = double(img.ptr<uchar>(j-1)[i-1] + 2 * img.ptr<uchar>(j-1)[i] + img.ptr<uchar>(j-1)[i+1] - img.ptr<uchar>(j+1)[i-1] - 2 * img.ptr<uchar>(j+1)[i] - img.ptr<uchar>(j+1)[i+1]);
            double gradX = double(img.ptr<uchar>(j-1)[i+1] + 2 * img.ptr<uchar>(j)[i+1] + img.ptr<uchar>(j+1)[i+1] - img.ptr<uchar>(j-1)[i-1] - 2 * img.ptr<uchar>(j)[i-1] - img.ptr<uchar>(j+1)[i-1]);
 
            gradXY.ptr<uchar>(j)[i] = sqrt(gradX*gradX + gradY*gradY); //计算梯度
            theta.ptr<uchar>(j)[i] = atan(gradY/gradX); //计算梯度方向
        }
    }
}
 
 
/**
 局部非极大值抑制
 gradXY 输入的梯度幅值
 theta 输入的梯度方向
 dst 输出的经局部非极大值抑制后的图像
 */
void nonLocalMaxValue (Mat &gradXY, Mat &theta, Mat &dst) {
    dst = gradXY.clone();
    for (int j = 1; j < gradXY.rows-1; j++) {
        for (int i = 1; i < gradXY.cols-1; i++) {
            double t = double(theta.ptr<uchar>(j)[i]);
            double g = double(dst.ptr<uchar>(j)[i]);
            if (g == 0.0) {
                continue;
            }
            double g0, g1;
            if ((t >= -(3*M_PI/8)) && (t < -(M_PI/8))) {
                g0 = double(dst.ptr<uchar>(j-1)[i-1]);
                g1 = double(dst.ptr<uchar>(j+1)[i+1]);
            }
            else if ((t >= -(M_PI/8)) && (t < M_PI/8)) {
                g0 = double(dst.ptr<uchar>(j)[i-1]);
                g1 = double(dst.ptr<uchar>(j)[i+1]);
            }
            else if ((t >= M_PI/8) && (t < 3*M_PI/8)) {
                g0 = double(dst.ptr<uchar>(j-1)[i+1]);
                g1 = double(dst.ptr<uchar>(j+1)[i-1]);
            }
            else {
                g0 = double(dst.ptr<uchar>(j-1)[i]);
                g1 = double(dst.ptr<uchar>(j+1)[i]);
            }
            
            if (g <= g0 || g <= g1) {
                dst.ptr<uchar>(j)[i] = 0.0;
            }
        }
    }
}
 
 
/**
 弱边缘点补充连接强边缘点
 img 弱边缘点补充连接强边缘点的输入和输出图像
 */
void doubleThresholdLink (Mat &img) {
    // 循环找到强边缘点，把其领域内的弱边缘点变为强边缘点
    for (int j = 1; j < img.rows-2; j++) {
        for (int i = 1; i < img.cols-2; i++) {
            // 如果该点是强边缘点
            if (img.ptr<uchar>(j)[i] == 255) {
                // 遍历该强边缘点领域
                for (int m = -1; m < 1; m++) {
                    for (int n = -1; n < 1; n++) {
                        // 该点为弱边缘点（不是强边缘点，也不是被抑制的0点）
                        if (img.ptr<uchar>(j+m)[i+n] != 0 && img.ptr<uchar>(j+m)[i+n] != 255) {
                            img.ptr<uchar>(j+m)[i+n] = 255; //该弱边缘点补充为强边缘点
                        }
                    }
                }
            }
        }
    }
    
    for (int j = 0; j < img.rows-1; j++) {
        for (int i = 0; i < img.cols-1; i++) {
            // 如果该点依旧是弱边缘点，及此点是孤立边缘点
            if (img.ptr<uchar>(j)[i] != 255 && img.ptr<uchar>(j)[i] != 255) {
                img.ptr<uchar>(j)[i] = 0; //该孤立弱边缘点抑制
            }
        }
    }
}
 
 
/**
 用双阈值算法检测和连接边缘
 low 输入的低阈值
 high 输入的高阈值
 img 输入的原图像
 dst 输出的用双阈值算法检测和连接边缘后的图像
 */
void doubleThreshold (double low, double high, Mat &img, Mat &dst) {
    dst = img.clone();
    
    // 区分出弱边缘点和强边缘点
    for (int j = 0; j < img.rows-1; j++) {
        for (int i = 0; i < img.cols-1; i++) {
            double x = double(dst.ptr<uchar>(j)[i]);
            // 像素点为强边缘点，置255
            if (x > high) {
                dst.ptr<uchar>(j)[i] = 255;
            }
            // 像素点置0，被抑制掉
            else if (x < low) {
                dst.ptr<uchar>(j)[i] = 0;
            }
        }
    }
    
    // 弱边缘点补充连接强边缘点
    doubleThresholdLink(dst);
}
 
 
int main () {
    Mat img = imread("woman.jpg", IMREAD_GRAYSCALE); //从文件中加载灰度图像
 
    // 读取图片失败，则停止
    if (img.empty()) {
        printf("读取图像文件失败");
        system("pause");
        return 0;
    }
    
    // 高斯滤波
    Mat gauss_img;
    gaussianFilter(img, gauss_img); //高斯滤波器
    
    // 用一阶偏导有限差分计算梯度幅值和方向
    Mat gradXY, theta;
    getGrandient(gauss_img, gradXY, theta);
    
    // 局部非极大值抑制
    Mat local_img;
    nonLocalMaxValue(gradXY, theta, local_img);
    
    // 用双阈值算法检测和连接边缘
    Mat dst;
    doubleThreshold(40, 80, local_img, dst);
 
    // 图像显示
    Mat outImg;
    mergeImg (outImg,img,dst); //图像拼接
    namedWindow("img");
    imshow("img",outImg);// 图像显示
    imwrite("canny算法.jpg", outImg);
 
    waitKey(); //等待键值输入
    return 0;
}