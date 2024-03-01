#include <opencv2/opencv.hpp>  
#include <iostream>  
  
int main(int argc, char** argv) {  
    // 检查是否提供了摄像头ID作为命令行参数  
    if (argc != 2) {  
        std::cerr << "Usage: " << argv[0] << " <camera_id>" << std::endl;  
        return -1;  
    }  
  
    // 尝试打开指定ID的摄像头  
    int cameraId = std::stoi(argv[1]);  
    cv::VideoCapture cap(cameraId);  
  
    // 检查摄像头是否成功打开  
    if (!cap.isOpened()) {  
        std::cerr << "Error: Could not open camera with ID " << cameraId << std::endl;  
        return -1;  
    }  
  
    // 创建一个窗口来显示摄像头视频  
    cv::namedWindow("Camera Feed", cv::WINDOW_AUTOSIZE);  
  
    // 循环读取摄像头的帧并显示  
    while (true) {  
        cv::Mat frame;  
  
        // 从摄像头读取一帧  
        if (!cap.read(frame)) {  
            std::cerr << "Error: Could not read frame from camera." << std::endl;  
            break;  
        }  
  
        // 显示帧  
        cv::imshow("Camera Feed", frame);  
  
        // 等待30毫秒，或者直到用户按下'q'键  
        if (cv::waitKey(30) == 'q') {  
            break;  
        }  
    }  
  
    // 释放摄像头资源  
    cap.release();  
  
    // 关闭所有OpenCV窗口  
    cv::destroyAllWindows();  
  
    return 0;  
}