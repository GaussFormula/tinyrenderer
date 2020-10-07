#pragma once
#include <vector>
#include <cassert>
#include <cmath>
#include <iostream>
#include "MathLibrary.h"

namespace MathLibrary
{
    template<> template<>
    vector<3,int>::vector(const vector<3,float>& v)
        :x(int(v.x + 0.5f)), y(int(v.y + 0.5f)), z(int(v.z + 0.5f))
    {}

    template<> template<>
    vector<3,float>::vector(const vector<3,int>& v)
        : x(v.x), y(v.y), z(v.z)
    {}

    template<> template<>
    vector<2,int>::vector(const vector<2,float>&v)
        :x(int(v.x+0.5f)),y(int(v.y+0.5f))
    {}

    template<> template<>
    vector<2,float>::vector(const vector<2,int>&v)
        :x(v.x),y(v.y)
    {}
}
