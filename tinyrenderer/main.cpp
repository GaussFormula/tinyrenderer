#include <limits>
#include <iostream>
#include <vector>

#include "MathLibrary.h"
#include "TgaImage.h"
#include "Model.h"

constexpr int width = 1000;
constexpr int height = 1280;
static std::uint64_t count = 0;


int main(int argc, char** argv)
{
    MathLibrary::vector<3, int> v1(1, 2, 3);
    std::cout << v1 + v1;
    return 0;
}