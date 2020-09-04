#include <limits>
#include <iostream>
#include <vector>

#include "MathLibrary.h"

constexpr int width = 800;
constexpr int height = 600;

int main(int argc, char** argv)
{
    std::vector<double> zbuffer(width * height, -std::numeric_limits<double>::max());
    MathLibrary::vector<3> v(1,1,1);
    v = v + v;
    std::cout << v/3 << std::endl;

    return 0;
}