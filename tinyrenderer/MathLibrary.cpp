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
        assert(cols == rhs.nrows());
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

    Matrix Matrix::transpose()
    {
        Matrix result(cols, rows);
        for (int i = 0; i < rows; ++i)
        {
            for (int j = 0; j < cols; ++j)
            {
                result[j][i] = m[i][j];
            }
        }
        return result;
    }

    Matrix Matrix::inverse() {
        assert(rows == cols);
        // augmenting the square matrix with the identity matrix of the same dimensions a => [ai]
        Matrix result(rows, cols * 2);
        for (int i = 0; i < rows; i++)
            for (int j = 0; j < cols; j++)
                result[i][j] = m[i][j];
        for (int i = 0; i < rows; i++)
            result[i][i + cols] = 1;
        // first pass
        for (int i = 0; i < rows - 1; i++) {
            // normalize the first row
            for (int j = result.cols - 1; j >= 0; j--)
                result[i][j] /= result[i][i];
            for (int k = i + 1; k < rows; k++) {
                float coeff = result[k][i];
                for (int j = 0; j < result.cols; j++) {
                    result[k][j] -= result[i][j] * coeff;
                }
            }
        }
        // normalize the last row
        for (int j = result.cols - 1; j >= rows - 1; j--)
            result[rows - 1][j] /= result[rows - 1][rows - 1];
        // second pass
        for (int i = rows - 1; i > 0; i--) {
            for (int k = i - 1; k >= 0; k--) {
                float coeff = result[k][i];
                for (int j = 0; j < result.cols; j++) {
                    result[k][j] -= result[i][j] * coeff;
                }
            }
        }
        // cut the identity matrix back
        Matrix truncate(rows, cols);
        for (int i = 0; i < rows; i++)
            for (int j = 0; j < cols; j++)
                truncate[i][j] = result[i][j + cols];
        return truncate;
    }

    std::ostream& operator<<(std::ostream& out,const Matrix& rhs)
    {
        for (int i = 0; i < rhs.nrows(); ++i)
        {
            for (int j = 0; j < rhs.ncols(); ++j)
            {
                
                out << const_cast<Matrix&>(rhs)[i][j];
                if (j < rhs.ncols() - 1)
                {
                    out << "\t";
                }
            }
            out << "\n";
        }
        return out;
    }
}
