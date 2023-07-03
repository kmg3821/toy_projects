#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>

#include "utils.h"

int img_h = 192;
int img_w = 168;

int main()
{
    auto path = std::filesystem::current_path();
    path.append("data/CroppedYale/");

    Eigen::MatrixXd U;
    Eigen::VectorXd avg;

    if(std::filesystem::exists(std::filesystem::path(path).append("Udata")) == false)
    {
        auto X = make_X_from_all(path);
        std::cout << X.rows() << "x" << X.cols() << '\n';

        avg = X.rowwise().mean();
        std::cout << avg.rows() << "x" << avg.cols() << '\n';

        const auto var = X.colwise() - avg;

        std::cout << "comput SVD...\n";
        const auto svd = compute_SVD(var);
        std::cout << "done\n";
        
        U = svd.matrixU();
        //S = svd.singularValues();
        //V = svd.matrixV();
        std::cout << U.rows() << "x" << U.cols() << '\n';

        std::ofstream file(std::filesystem::path(path).append("Udata"), std::ios::binary);
        file << U.cols();
        file.write(reinterpret_cast<const char*>(U.data()), sizeof(double) * img_h * img_w * U.cols())
            .write(reinterpret_cast<const char*>(avg.data()), sizeof(double) * img_h * img_w);
        std::cout << "saved\n";
        file.close();
    }
    else
    {
        std::ifstream file(std::filesystem::path(path).append("Udata"), std::ios::binary);
        if(!file.is_open())
        {
            std::cout << "fail to open\n";
            exit(1);
        }
        int sz;
        file >> sz;
        U = Eigen::MatrixXd(img_h * img_w, sz);
        avg = Eigen::VectorXd(img_h * img_w);
        file.read(reinterpret_cast<char*>(U.data()), sizeof(double) * img_h * img_w * sz)
            .read(reinterpret_cast<char*>(avg.data()), sizeof(double) * img_h * img_w);
        file.close();
    }

    const auto min_val = U.colwise().minCoeff();
    const auto max_val = U.colwise().maxCoeff();
    U = (((U.array().rowwise() - min_val.array()) * 255.0).rowwise() / (max_val.array() - min_val.array())).eval();
    std::cout << U.rows() << "x" << U.cols() << "\n";

    show_image(avg, "./out/mean.jpg");
    for(int i = 0; i < 10; ++i)
        show_image(U.col(i), "./out/" + std::to_string(i) + ".jpg");
    

    return 0;
}