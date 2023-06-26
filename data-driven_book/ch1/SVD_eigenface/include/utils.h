#include <string>

#include "Eigen/Dense"
#include "Eigen/SVD"

using SVDResult = Eigen::BDCSVD<Eigen::MatrixXd, Eigen::ComputeThinU>;

// X 만드는 함수
Eigen::MatrixXd make_X_from_all(const std::string);

// 주어진 사진 한장 보여주기
void show_image(const std::string);

// svd
SVDResult compute_SVD(const Eigen::MatrixXd);