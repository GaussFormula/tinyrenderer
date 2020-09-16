#include "MathLibrary.h"

namespace MathLibrary
{
    template<typename T>
    vector<3, T> cross(const vector<3, T>& v1, const vector<3, T>& v2)
    {
        vector<3, T> ret;
        ret[0] = v1[1] * v2[2] - v1[2] * v2[1];
        ret[1] = v1[2] * v2[0] - v1[0] * v2[2];
        ret[2] = v1[0] * v2[1] - v1[1] * v2[0];
        return ret;
        //{ v1[1] * v2[2] - v1[2] * v2[1], v1[2] * v2[0] - v1[0] * v2[2], v1[0] * v2[1] - v1[1] * v2[0] };
    }
}