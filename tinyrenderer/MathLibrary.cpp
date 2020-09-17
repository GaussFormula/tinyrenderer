#include "MathLibrary.h"

namespace MathLibrary
{

    vector<3, int> cross(const vector<3, int>& v1, const vector<3, int>& v2)
    {
        vector<3, int> ret;
        ret[0] = v1[1] * v2[2] - v1[2] * v2[1];
        ret[1] = v1[2] * v2[0] - v1[0] * v2[2];
        ret[2] = v1[0] * v2[1] - v1[1] * v2[0];
        return ret;
        //{ v1[1] * v2[2] - v1[2] * v2[1], v1[2] * v2[0] - v1[0] * v2[2], v1[0] * v2[1] - v1[1] * v2[0] };
    }

	template<> template<>
	vector3f::vector(const vector3i& rhs)
	{
		this->x = rhs.x;
		this->y = rhs.y;
		this->z = rhs.z;
	}

	template<> template<>
	vector3i::vector(const vector3f& rhs)
	{
		this->x = rhs.x + 0.5f;
		this->y = rhs.y + 0.5f;
		this->z = rhs.z + 0.5f;
	}
}