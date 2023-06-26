#include <iostream>
#include <filesystem>

#include "utils.h"

int main()
{
    auto path = std::filesystem::current_path();
    path.append("data/CroppedYale/");
    auto X = make_X_from_all(path);

    std::cout << X.rows() << "x" << X.cols() << '\n';

    std::cout << X.topLeftCorner(30,30) << '\n';

    //show_image("data/CroppedYale/yaleB01/yaleB01_P00A-005E-10.pgm");

    

    return 0;
}