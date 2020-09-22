#include <vector>
#include <cassert>
#include <cmath>
#include <iostream>
#include "MathLibrary.h"

namespace MathLibrary
{
    template<class T>
    MathLibrary::vector3<T> cross(const MathLibrary::vector3<T>& lhs, const MathLibrary::vector3<T>& rhs)
    {
        return MathLibrary::vector3<T>(lhs.y * rhs.z - lhs.z * rhs.y, lhs.z * rhs.x - lhs.x * rhs.z, lhs.x * rhs.y - lhs.y * rhs.x);
    }
}
