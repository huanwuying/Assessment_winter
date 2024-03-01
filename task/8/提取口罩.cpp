#include <opencv2/opencv.hpp>  
#include <iostream>  
  
int main(int argc, char** argv) {  
    // 确保提供了输入和输出文件路径  
    if (argc != 3) {  
        std::cerr << "Usage: " << argv[0] << " <input_image> <output_region>" << std::endl;  
        return -1;  
    }  
  
    // 读取输入图像  
    cv::Mat image = cv::imread(argv[1], cv::IMREAD_COLOR);  
    if (image.empty()) {  
        std::cerr << "Error: Could not read the input image: " << argv[1] << std::endl;  
        return -1;  
    }  
  
    // 指定要提取的区域（左上角和右下角坐标）  
    cv::Rect region(50, 50, 200, 200); // 从(50,50)开始，宽度为200，高度为200  
  
    // 提取区域  
    cv::Mat extractedRegion = image(region);  
  
    // 保存提取的区域到文件  
    cv::imwrite(argv[2], extractedRegion);  
  
    // 显示原始图像和提取的区域  
    cv::namedWindow("Original Image", cv::WINDOW_AUTOSIZE);  
    cv::imshow("Original Image", image);  
  
    cv::namedWindow("Extracted Region", cv::WINDOW_AUTOSIZE);  
    cv::imshow("Extracted Region", extractedRegion);  
  
    // 等待按键并关闭窗口  
    cv::waitKey(0);  
  
    return 0;  
}