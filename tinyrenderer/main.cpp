#include <limits>
#include <iostream>
#include <vector>

#include "MathLibrary.h"
#include "TgaImage.h"
#include "Model.h"

const int width = 800;
const int height = 800;
const int depth = 255;

MathLibrary::vector3f lightDir(0, 0, -1);
MathLibrary::vector3f camera(0, 0, 3);



int main()
{
    MathLibrary::vector3<int> v1(0, 2, 0);
    MathLibrary::vector3<float> v2(v1);
    std::cout << v2;
    return 0;
}