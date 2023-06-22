#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <Eigen/Dense>
#include <Eigen/SVD>

// g++ ./main.cpp -o main -std=c++17 -O2 -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_imgcodecs

int main()
{
    cv::Mat tmp = cv::imread("pigeon.jpg", cv::IMREAD_GRAYSCALE);
    cv::Mat img0;
    cv::resize(tmp,img0,cv::Size(640,480));

    Eigen::MatrixXd mat0(640,480);
    for(int j = 0; j < 480; ++j)
        for(int i = 0; i < 640; ++i)
            mat0(i,j) = img0.at<uint8_t>(j,i);

    Eigen::BDCSVD<Eigen::MatrixXd, Eigen::ComputeThinU | Eigen::ComputeThinV> svd(mat0);
    auto U = svd.matrixU();
    auto V = svd.matrixV();
    auto S = svd.singularValues();

    Eigen::MatrixXd mat(640,480);
    mat.setZero();
    for(int i = 0; i < 20; ++i)
        mat += S(i)*(U.col(i)*V.col(i).transpose());

    cv::Mat img(img0.size(),img0.type());
    for(int j = 0; j < 480; ++j)
        for(int i = 0; i < 640; ++i)
            img.at<uint8_t>(j,i) = static_cast<uint8_t>(mat(i,j));

    cv::imshow("original pigeon", img0);
    cv::waitKey(10);
    cv::imshow("compressed pigeon", img);
    cv::waitKey(10);

    std::cout << "Press enter to exit...";
    getchar();

    return 0; 
}