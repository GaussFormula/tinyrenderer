#pragma once
#include <vector>
#include <cassert>
#include <cmath>
#include <iostream>
#include "MathLibrary.h"

namespace MathLibrary
{
    /*template<class T>
    MathLibrary::vector3<T> cross(const MathLibrary::vector3<T>& lhs, const MathLibrary::vector3<T>& rhs)
    {
        return MathLibrary::vector3<T>(lhs.y * rhs.z - lhs.z * rhs.y, lhs.z * rhs.x - lhs.x * rhs.z, lhs.x * rhs.y - lhs.y * rhs.x);
    }*/

    template<> template<>
    vector3<int>::vector3(const vector3<float>& v)
        :x(int(v.x + 0.5f)), y(int(v.y + 0.5f)), z(int(v.z + 0.5f))
    {}

    template<> template<>
    vector3<float>::vector3(const vector3<int>& v)
        : x(v.x), y(v.y), z(v.z)
    {}


}
