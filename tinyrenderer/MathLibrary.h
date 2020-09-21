#pragma once

#include <cmath>
#include <iostream>

namespace MathLibrary
{
    template<typename T>
    class vector2
    {
    public:
        T x, y;
        vector2<T>():x(T()),y(T()){}
        vector2<T>(T _x,T _y)
            :x(_x),y(_y)
        {}

        vector2<T> (const vector2<T>& rhs)
            :x(rhs.x),y(rhs.y)
        {}

        vector2<T>& operator=(const vector2<T>& rhs)
        {
            if (this != &rhs)
            {
                x = rhs.x;
                y = rhs.y;
            }
            return *this;
        }

        vector2<T> operator+(const vector2<T>& rhs)const 
        {
            return vector2<T>(x + rhs.x, y + rhs.y);
        }

        vector2<T> operator-(const vector2<T>& rhs)const
        {
            return vector2<T>(x - rhs.x, y - rhs.y);
        }

        vector2<T> operator*(float f)const
        {
            return vector2<T>(x * f, y * f);
        }

        T& operator[](const int i)
        {
            if (i <= 0)
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
    std::ostream& operator<<(std::ostream& out, vector2<T>& v)
    {
        out << v.x << " " << v.y << "\n";
    }

    template<typename T>
    class vector3
    {
    public:
        T x, y, z;
        vector3<T>():x(T()),y(T()),z(T()){}
        vector3<T>(T _x,T _y,T _z)
            :x(_x),y(_y),z(_z)
        {}
        vector3<T>(const vector3<T>& rhs)
            : x(T()), y(T()), z(T())
        {
            *this = rhs;
        }
        
        template<typename U>
        vector3<T>(const vector3<U>& v);

        vector3<T>& operator=(const vector3<T>& rhs)
        {
            if (this != &rhs)
            {
                x = rhs.x;
                y = rhs.y;
                z = rhs.z;
            }
            return *this;
        }

        vector3<T> operator+(const vector3<T>& rhs)const
        {
            return vector3<T>(x + rhs.x, y + rhs.y, z + rhs.z);
        }

        vector3<T> operator-(const vector3<T>& rhs)const
        {
            return vector3<T>(x - rhs.x, y - rhs.y, z - rhs.z);
        }

        vector3<T> operator*(float f)const
        {
            return vector3<T>(x * f, y * f, z * f);
        }

        T operator*(const vector3<T>& rhs)const
        {
            return x * rhs.x + y * rhs.y + z * rhs.z;
        }

        float norm()const
        {
            return std::sqrt(x * x + y * y + z * z);
        }

        vector3<T>& normalize(T l = 1)
        {
            *this = (*this) * (l / norm());
            return *this;
        }

        T& operator[](const int i)
        {
            if (i <= 0)
            {
                return x;
            }
            else if(i==1)
            {
                return y;
            }
            else
            {
                return z;
            }
        }
    };

    template<typename T>
    std::ostream& operator<<(std::ostream& out, const vector3<T>& rhs)
    {
        out << rhs.x << " " << rhs.y << " " << rhs.z << "\n";
        return out;
    }

    template<class T>
    MathLibrary::vector3<T> cross(const MathLibrary::vector3<T>& lhs, const MathLibrary::vector3<T>& rhs)
    {
        return MathLibrary::vector3<T>(lhs.y * rhs.z - lhs.z * rhs.y, lhs.z * rhs.x - lhs.x * rhs.z, lhs.x * rhs.y - lhs.y * rhs.x);
    }
}
