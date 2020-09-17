#pragma once

#include <cmath>
#include <cassert>
#include <iostream>

namespace MathLibrary
{
    template <typename T>
    class vector2
    {
    public:
        T x, y;
        vector2<T>() : x(T()), y(T())
        {

        }

        vector2<T>(T _x,T _y):x(_x),y(_y){}

        vector2<T>(const vector2<T>& v) : x(T()), y(T())
        {
            *this = v;
        }

        vector2<T>& operator=(const vector2<T>& v)
        {
            if (this != v)
            {
                x = v.x;
                y = v.y;
            }
            return *this;
        }

        vector2<T> operator+(const vector2<T>& v)const
        {
            return vector2<T>(x + v.x, y + v.y);
        }

        vector2<T> operator-(const vector2<T>& v)const
        {
            return vector2<T>(x - v.x, y - v.y);
        }

        vector2<T> operator*(const float f)
        {
            return vector2<T>(x * f, y * f);
        }

        T& operator[](const int i)
        {
            if (x <= 0)
            {
                return x;
            }
            else
            {
                return y;
            }
        }
    };

    template<typename T>
    T operator*(const vector2<T>& lhs, const vector2<T>& rhs)
    {
        T ret;
        for (int i = 0; i < 2; ++i)
        {
            ret += lhs[i] * rhs[i];
        }
        return ret;
    }

    template<typename T>
    std::ostream& operator<<(const std::ostream&out, const vector2<T>& rhs)
    {
        out << rhs[0] << " " << rhs[1] << " ";
        return out;
    }

    template<typename T>
    class vector3
    {
    public:
        T x, y, z;
        vector3<T>():x(T()),y(T()),z(T()){}
        vector3<T>(T _x,T _y,T _z)
            :x(_x),y(_y),z(_z){}

        template<typename U>
        vector3<T>(const vector3<U>& v);

        vector3<T>(const vector3<T>& v)
            :x(T()),y(T()),z(T())
        {
            *this = v;
        }

        vector3<T>& operator=(const vector3<T>& v)
        {
            if (this != &v)
            {
                x = v.x;
                y = v.y;
                z = v.z;
            }
            return *this;
        }

        vector3<T> operator+(const vector3<T>& rhs)
        {
            return vector3<T>(x + rhs.x, y + rhs.y, z + rhs.z);
        }

        vector3<T> operator-(const vector3<T>& rhs)
        {
            return vector3<T>(x - rhs.x, y - rhs.y, z - rhs.z);
        }
    };

    typedef vector2<int> vector2i;
    typedef vector2<float> vector2f;
    typedef vector3<int> vector3i;
    typedef vector3<float> vector3f;
}