#ifndef __UTILS_H__
#define __UTILS_H__

#include <string>

#include "Eigen/Dense"
#include "Eigen/SVD"

extern int img_h;
extern int img_w;

using SVDResult = Eigen::BDCSVD<Eigen::MatrixXd, Eigen::ComputeThinU | Eigen::ComputeThinV>;

Eigen::MatrixXd make_X_from_all(const std::string);

void show_image(const std::string);
void show_image(const Eigen::MatrixXd&, const std::string = "");

// svd
SVDResult compute_SVD(const Eigen::MatrixXd&);

#endif