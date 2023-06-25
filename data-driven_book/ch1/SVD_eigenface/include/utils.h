#include "Eigen/Dense"
#include "Eigen/SVD"
#include <string>

using SVDResult = Eigen::BDCSVD<Eigen::MatrixXd, Eigen::ComputeThinU>;

// X 만드는 함수
Eigen::MatrixXd make_X_from_all(std::string);

// 주어진 사진 한장 보여주기
void show_image(std::string);

// svd
SVDResult compute_SVD(Eigen::MatrixXd);