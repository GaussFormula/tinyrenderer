#include <limits>
#include <iostream>
#include <vector>

#include "MathLibrary.h"

constexpr int width = 800;
constexpr int height = 600;

int main(int argc, char** argv)
{
    std::vector<double> zbuffer(width * height, -std::numeric_limits<double>::max());
    MathLibrary::Matrix<2, 2> m;
    m[0] = MathLibrary::vector<2>{ 6,7 };
    m[1] = MathLibrary::vector<2>{ 0,3 };
    
    std::cout << m.det();

    return 0;
}