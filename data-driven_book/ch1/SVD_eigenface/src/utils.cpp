#include <iostream>
#include <filesystem>

#include "utils.h"
#include "opencv2/opencv.hpp"
#include "Eigen/Dense"
#include "Eigen/SVD"

static int img_h;
static int img_w;

void show_image(const std::string path)
{
    const cv::Mat img = cv::imread(path, cv::IMREAD_GRAYSCALE);
    cv::imshow("", img);
    cv::waitKey(0);
}

void show_image(const Eigen::MatrixXd& v)
{
    cv::Mat img(1, 32256, CV_8U);

    for(int i = 0; i < v.rows(); ++i)
    {
        img.at<uint8_t>(0,i) = static_cast<uint8_t>(v(i,0));
    }
    img = img.reshape(0,192);
    
    cv::imshow("", img);
    cv::waitKey(0);
}

Eigen::MatrixXd make_X_from_all(const std::string path)
{
    Eigen::MatrixXd X(0,0);
    int cnt = 0;
    for(const auto& file : std::filesystem::recursive_directory_iterator{path})
    {
        if(file.path().extension() != ".pgm") continue;
        cv::Mat img = cv::imread(file.path(), cv::IMREAD_GRAYSCALE);
        img_h = img.size().height;
        img_w = img.size().width;
        
        img = img.reshape(0,1);
        cnt++;

        const int sz = img.size().width;
        if(X.rows() != 0 && X.rows() != sz)
        {
            std::cerr << "dimension does not admit\n";
            exit(1);
        }

        X.conservativeResize(sz, cnt);
        for(int i = 0; i < sz; ++i)
        {
            X(i, cnt-1) = img.at<uint8_t>(0,i);
        }
    }

    return X;
}

SVDResult compute_SVD(const Eigen::MatrixXd& X)
{
    return SVDResult(X);
}