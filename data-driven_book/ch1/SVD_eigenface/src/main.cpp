#include <iostream>
#include <filesystem>
#include <fstream>

#include "utils.h"

int main()
{
    auto path = std::filesystem::current_path();
    path.append("data/CroppedYale/");

    Eigen::MatrixXd U;

    if(std::filesystem::exists(std::filesystem::path(path).append("Udate.txt")) == false)
    {
        auto X = make_X_from_all(path);
        std::cout << X.rows() << "x" << X.cols() << '\n';

        const auto avg = X.rowwise().mean();
        std::cout << avg.rows() << "x" << avg.cols() << '\n';

        const auto var = X.colwise() - avg;
        std::cout << var.rows() << "x" << var.cols() << '\n';

        std::cout << "comput SVD...\n";
        const auto svd = compute_SVD(var);
        std::cout << "done\n";
        
        const auto& U = svd.matrixU();
        //const auto& S = svd.singularValues();
        //const auto& V = svd.matrixV();

        std::cout << U.rows() << "x" << U.cols() << '\n';

        std::ofstream file(std::filesystem::path(path).append("Udate.txt"));
        file << U.rows() << " " << U.cols() << '\n';
        file << U.transpose();
        std::cout << "saved\n";

        show_image(U.col(0) * 10000.0);
    }
    else
    {
        freopen(std::filesystem::path(path).append("Udate.txt").string().data(), "r", stdin);

        Eigen::MatrixXd U(32256,1);

        {
            int tmp;
            std::cin >> tmp >> tmp;
        }
        
        for(int i = 0; i < 4; ++i)
        {
            for(int i = 0; i < 32256; ++i)
            {
                double x;
                std::cin >> x;
                U(i,0) = x;
            }
        }

        double min_val = U.col(0).minCoeff();
        double max_val = U.col(0).maxCoeff();

        U = (U.array() - min_val) * 255.0 / (max_val - min_val);

        show_image(U.col(0));
    }


    

    // std::cout << U.rows() << "x" << U.cols() << '\n';
    // std::cout << U.col(0) << "\n";

    //show_image(avg);
    //show_image(U.col(0) * 10000.0);

    //show_image("data/CroppedYale/yaleB01/yaleB01_P00A-005E-10.pgm");

    

    return 0;
}