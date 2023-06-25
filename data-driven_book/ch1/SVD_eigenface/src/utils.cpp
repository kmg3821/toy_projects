#include "utils.h"
//#include "opencv2/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>

void show_image(std::string path)
{
    cv::Mat img = cv::imread(path, cv::IMREAD_GRAYSCALE);
    cv::imshow("", img);
    cv::waitKey(0);
}