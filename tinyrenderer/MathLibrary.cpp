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

    Matrix::Matrix(int r,int c):
        m(std::vector<std::vector<float>>(r,std::vector<float>(c,0.0f))),
        rows(r),cols(c)
    {}

    Matrix Matrix::identity(int dimensions)
    {
        Matrix E(dimensions, dimensions);
        for (int i = 0; i < dimensions; ++i)
        {
            for (int j = 0; j < dimensions; j++)
            {
                E[i][j] = (i == j) ? 1.0f : 0.0f;
            }
        }
        return E;
    }

    int Matrix::nrows()const
    {
        return rows;
    }

    int Matrix::ncols()const
    {
        return cols;
    }

    std::vector<float>& Matrix::operator[](const int i)
    {
        assert(i >= 0 && i < rows);
        return m[i];
    }

    Matrix Matrix::operator*(const Matrix& rhs)
    {
        Matrix ret(rows, rhs.ncols());
        for (int i = 0; i < rows; ++i)
        {
            for (int j = 0; j < rhs.ncols(); ++j)
            {
                ret[i][j] = 0.0f;
                for (int k = 0; k < cols; ++k)
                {
                    ret[i][j] += m[i][k] * rhs.m[k][j];
                }
            }
        }
        return ret;
    }
}
