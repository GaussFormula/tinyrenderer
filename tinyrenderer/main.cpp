#include <limits>
#include <iostream>
#include <vector>

const int width = 800;
const int height = 600;

int main(int argc, char** argv)
{
    std::vector<double> zbuffer(width * height, -std::numeric_limits<double>::max());

    return 0;
}