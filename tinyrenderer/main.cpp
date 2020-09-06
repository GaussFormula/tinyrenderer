#include <limits>
#include <iostream>
#include <vector>

#include "MathLibrary.h"

constexpr int width = 800;
constexpr int height = 600;

int main(int argc, char** argv)
{
    std::vector<double> zbuffer(width * height, -std::numeric_limits<double>::max());
    MathLibrary::Matrix<3, 3> m;
    m[0] = MathLibrary::vector<3>(1, 2, 3);
    m[1] = MathLibrary::vector<3>(5, 3, 1);
    m[2] = MathLibrary::vector<3>(4, 2, 3);
    std::cout << m.invert();

    return 0;
}