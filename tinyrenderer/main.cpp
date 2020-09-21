#include <limits>
#include <iostream>
#include <vector>

#include "MathLibrary.h"
#include "TgaImage.h"
#include "Model.h"

template<typename T>
MathLibrary::vector3<T> cross(const MathLibrary::vector3<T>& lhs, const MathLibrary::vector3<T>& rhs)
{
    return MathLibrary::vector3<T>(lhs.y * rhs.z - lhs.z * rhs.y, lhs.z * rhs.x - lhs.x * rhs.z, lhs.x * rhs.y - lhs.y * rhs.x);
}

int main()
{
    MathLibrary::vector3<int> v1(0, 2, 0);
    MathLibrary::vector3<int> v2(3, 0, 0);
    std::cout << cross(v1,v2).normalize();
    return 0;
}