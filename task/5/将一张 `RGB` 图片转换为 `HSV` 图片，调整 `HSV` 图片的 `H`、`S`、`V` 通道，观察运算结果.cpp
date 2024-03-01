#include <opencv2/opencv.hpp>  
#include <iostream>  
  
int main(int argc, char** argv) {  
    // 确保提供了输入和输出文件路径  
    if (argc != 3) {  
        std::cerr << "Usage: " << argv[0] << " <input_image> <output_image>" << std::endl;  
        return -1;  
    }  
  
    // 读取输入图像  
    cv::Mat rgbImage = cv::imread(argv[1], cv::IMREAD_COLOR);  
    if (rgbImage.empty()) {  
        std::cerr << "Error: Could not read the input image: " << argv[1] << std::endl;  
        return -1;  
    }  
  
    // 创建一个HSV图像矩阵  
    cv::Mat hsvImage;  
  
    // 将RGB图像转换为HSV图像  
    cv::cvtColor(rgbImage, hsvImage, cv::COLOR_BGR2HSV);  
  
    // 调整整个图像，遍历所有像素。  
    for (int y = 0; y < hsvImage.rows; y++) {  
        for (int x = 0; x < hsvImage.cols; x++) {  
            cv::Vec3b& pixel = hsvImage.at<cv::Vec3b>(y, x);  
            // 增加色调  
            pixel[0] = (pixel[0] + 30) % 180; // HSV色调是循环的，范围是[0, 179]  
            // 减少饱和度  
            pixel[1] = std::max(0, pixel[1] - 50);  
            // 增加明度  
            pixel[2] = std::min(255, pixel[2] + 20);  
        }  
    }  
  
    // 将调整后的HSV图像转换回RGB图像以便显示  
    cv::Mat adjustedRGB;  
    cv::cvtColor(adjustedHSV, adjustedRGB, cv::COLOR_HSV2BGR);  
  
    // 显示原始和调整后的图像  
    cv::namedWindow("Original Image", cv::WINDOW_AUTOSIZE);  
    cv::imshow("Original Image", rgbImage);  
  
    cv::namedWindow("Adjusted HSV Image", cv::WINDOW_AUTOSIZE);  
    cv::imshow("Adjusted HSV Image", adjustedHSV);  
  
    cv::namedWindow("Adjusted RGB Image", cv::WINDOW_AUTOSIZE);  
    cv::imshow("Adjusted RGB Image", adjustedRGB);  
  
    // 等待用户按键，然后关闭窗口  
    cv::waitKey(0);  
  
    return 0;  
}