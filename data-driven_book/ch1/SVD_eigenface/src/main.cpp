#include <iostream>
#include "utils.h"
#include <filesystem>

int main()
{
    auto path = std::filesystem::current_path();
    path.append("data/CroppedYale/");
    for(const auto& x : std::filesystem::directory_iterator(path))
    {
        for(const auto& y : std::filesystem::directory_iterator(x.path()))
        {
            std::cout << y.path() << '\n';
        }
    }

    //show_image("data/CroppedYale/yaleB01/yaleB01_P00A-005E-10.pgm");

    

    return 0;
}