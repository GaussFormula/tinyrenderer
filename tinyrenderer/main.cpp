#include <limits>
#include <iostream>
#include <vector>

#include "MathLibrary.h"

constexpr int width = 800;
constexpr int height = 600;

int main(int argc, char** argv)
{
    std::vector<double> zbuffer(width * height, -std::numeric_limits<double>::max());
    MathLibrary::vector<3> v;
    std::cout << v.norm2() << std::endl;

    return 0;
}