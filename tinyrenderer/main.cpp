#include <limits>
#include <iostream>
#include <vector>

#include "MathLibrary.h"
#include "TgaImage.h"
#include "Model.h"

int main()
{
    MathLibrary::vector3<int> v1(0, 2, 0);
    MathLibrary::vector3<int> v2(3, 0, 0);
    std::cout << MathLibrary::cross(v1,v2).normalize();
    return 0;
}