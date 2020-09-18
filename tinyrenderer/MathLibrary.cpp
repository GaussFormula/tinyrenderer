#include "MathLibrary.h"

namespace MathLibrary
{
    template<> template<>
    vector3i::vector3(const vector3f& v)
        :x(int(v.x+0.5f)),y(int(v.y+0.5f)),z(int(v.z+0.5f))
    {

    }

    template<> template<>
    vector3f::vector3(const vector3i& v)
        :x(v.x),y(v.y),z(v.z)
    {

    }

    Matrix::Matrix(int rows,int cols)
        :m(std::vector<std::vector<float>>(rows,std::vector<float>(cols,0.0f))),
        rows(rows),cols(cols)
    {}

    int Matrix::nrows()
    {
        return rows;
    }

    int Matrix::ncols()
    {
        return cols;
    }

    Matrix Matrix::identity(int dimensions)
    {
        Matrix E(dimensions, dimensions);
        for (int i = 0; i < dimensions; ++i)
        {
            for (int j = 0; j < dimensions; ++i)
            {
                E[i][j] = (i == j) ? 1.0f : 0.0f;
            }
        }
        return E;
    }

    std::vector<float>& Matrix::operator[](const int i)
    {
        assert(i >= 0 && i < rows);
        return m[i];
    }

    Matrix Matrix::operator*(const Matrix& v)
    {
        assert(cols == v.rows);
        Matrix result(rows, v.cols);
        for (int i = 0; i < rows; ++i)
        {
            for (int j = 0; j < v.cols; ++j)
            {
                result[i][j] = 0.0f;
                for (int k = 0; k < cols; ++k)
                {
                    result[i][j] += m[i][k] * v.m[k][j];
                }
            }
        }
        return result;
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

    Matrix Matrix::inverse()
    {
        assert(rows == cols);
        // augmenting the square matrix with the identity matrix of the same dimensions a => [ai]
        Matrix result(rows, cols * 2);
        for (int i = 0; i < rows; ++i)
        {
            for (int j = 0; j < cols; ++j)
            {
                result[i][j] = m[i][j];
            }
        }
        for (int i = 0; i < rows; ++i)
        {
            result[i][i + cols] = 1;
        }

        // First pass.
        for (int i = 0; i < rows - 1; ++i)
        {
            // normalize the first row.
            for (int j = result.cols - 1; j >= 0; --j)
            {
                result[i][j] /= result[i][i];
            }
            for (int k = i + 1; k < rows; ++k)
            {
                float coeff = result[k][i];
                for (int j = 0; j < result.cols; ++j)
                {
                    result[k][j] -= result[i][j] * coeff;
                }
            }
        }

        // normalize the last row
        for (int j = result.cols - 1; j >= rows - 1; j--)
            result[rows - 1][j] /= result[rows - 1][rows - 1];
        // second pass
        for (int i = rows - 1; i > 0; i--) 
        {
            for (int k = i - 1; k >= 0; k--) 
            {
                float coeff = result[k][i];
                for (int j = 0; j < result.cols; j++) 
                {
                    result[k][j] -= result[i][j] * coeff;
                }
            }
        }

        // cut the identity matrix back.
        Matrix truncate(rows, cols);
        for (int i = 0; i < rows; ++i)
        {
            for (int j = 0; j < cols; ++j)
            {
                truncate[i][j] = result[i][j + cols];
            }
        }
        return truncate;
    }

    std::ostream& operator<<(std::ostream& out, Matrix& m)
    {
        for (int i = 0; i < m.nrows(); ++i)
        {
            for (int j = 0; j < m.ncols(); ++j)
            {
                out << m[i][j];
                if (j < m.ncols() - 1)
                {
                    out << "\t";
                }
            }
            out << "\n";
        }
        return out;
    }
}