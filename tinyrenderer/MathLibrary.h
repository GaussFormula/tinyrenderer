#pragma once

#include <cmath>
#include <vector>
#include <cassert>
#include <iostream>

namespace MathLibrary
{
    template<size_t DimCols, size_t DimRows, typename T> class Matrix;

    template<size_t DIM,typename T>
    class vector
    {
    private:
        T data[DIM];
    public:
        vector()
        {
            for (size_t i = 0; i < DIM; ++i)
            {
                data[i] = T();
            }
        }

        vector(const vector<DIM, T>& v)
        {
            if (this != &v)
            {
                for (size_t i = 0; i < DIM; ++i)
                {
                    this->data[i] = v[i];
                }
            }
        }

        T& operator[](const size_t i)
        {
            assert(i < DIM&& i >= 0);
            return data[i];
        }
        const T& operator[](const size_t i)const
        {
            assert(i < DIM&& i >= 0);
            return data[i];
        }

        vector<DIM, T>& operator=(const vector<DIM, T>& v)
        {
            if (this != &v)
            {
                for (size_t i = 0; i < DIM; ++i)
                {
                    data[i] = v[i];
                }
            }
            return *this;
        }
    };

    template<typename T>
    class vector<2, T>
    {
    public:
        T x, y;
        vector():x(T()),y(T()){}
        vector(T X,T Y):x(X),y(Y){}

        vector(const vector<2, T>& v)
        {
            if (this != &v)
            {
                x = v.x;
                y = v.y;
            }
        }

        template<typename U>
        vector<2, T>(const vector<2, U>& v);

        T& operator[](const size_t i)
        {
            assert(i >= 0 && i < 2);
            return i == 0 ? x : y;
        }
        const T& operator[](const size_t i)const
        {
            assert(i >= 0 && i < 2);
            return i == 0 ? x : y;
        }

        vector<2, T>& operator=(const vector<2, T>& v)
        {
            if (this != &v)
            {
                x = v.x;
                y = v.y;
            }
            return *this;
        }
    };

    template<typename T>
    class vector<3, T>
    {
    public:
        T x, y, z;
        vector():x(T()),y(T()),z(T()){}
        vector(T X,T Y,T Z):x(X),y(Y),z(Z){}

        template <typename U>
        vector<3, T>(const vector<3, U>& v);

        vector(const vector<3, T>& v)
        {
            if (this != &v)
            {
                x = v.x;
                y = v.y;
                z = v.z;
            }
        }

        T& operator[](const size_t i)
        {
            assert(i < 3);
            return i <= 0 ? x : (i == 1 ? y : z);
        }
        const T& operator[](const size_t i)const
        {
            assert(i < 3);
            return i <= 0 ? x : (i == 1 ? y : z);
        }

        vector<3, T>& operator=(const vector<3, T>& v)
        {
            if (this != &v)
            {
                x = v.x;
                y = v.y;
                z = v.z;
            }
            return *this;
        }

        float norm()const
        {
            return std::sqrt(x * x + y * y + z * z);
        }

        vector<3, T>& normalize(T l = 1)
        {
            *this = (*this) * (l / norm());
            return *this;
        }
    };

    template<size_t DIM,typename T>
    T operator*(const vector<DIM, T>& lhs, const vector<DIM, T>& rhs)
    {
        T ret = T();
        for (size_t i = 0; i < DIM; ++i)
        {
            ret = ret + lhs[i] * rhs[i];
        }
        return ret;
    }

    template<size_t DIM,typename T>
    vector<DIM, T> operator+(const vector<DIM, T>& lhs, const vector<DIM, T>& rhs)
    {
        vector<DIM, T> ret;
        for (size_t i = 0; i < DIM; ++i)
        {
            ret[i] = lhs[i] - rhs[i];
        }
        return ret;
    }

    template<size_t DIM,typename T>
    vector<DIM, T> operator-(const vector<DIM, T>& lhs, const vector<DIM, T>& rhs)
    {
        vector<DIM, T>ret;
        for (size_t i = 0; i < DIM; ++i)
        {
            ret[i] = lhs[i] - rhs[i];
        }
        return ret;
    }

    template<size_t DIM,typename T,typename U>
    vector<DIM, T> operator*(const vector<DIM, T>& lhs, const U& rhs)
    {
        vector<DIM, T> result;
        for (size_t i = 0; i < DIM; ++i)
        {
            result[i] = lhs[i] * rhs;
        }
        return result;
    }

    template<size_t DIM,typename T,typename U>
    vector<DIM, T> operator/(const vector<DIM, T>& lhs, const U& rhs)
    {
        vector<DIM, T> result;
        for (size_t i = 0; i < DIM; ++i)
        {
            result[i] = lhs[i] / rhs;
        }
        return result;
    }

    template<size_t LEN,size_t DIM,typename T>
    vector<LEN, T> embed(const vector<DIM, T>& v, T fill = 1)
    {
        vector<LEN, T> result;
        for (size_t i = 0; i < LEN; ++i)
        {
            result[i] = (i < DIM ? v[i] : fill);
        }
        return result;
    }

    template<size_t LEN,size_t DIM,typename T>
    vector<LEN, T> proj(const vector<DIM, T>& v)
    {
        vector<LEN, T> result;
        for (size_t i = 0; i < LEN; ++i)
        {
            result[i] = v[i];
        }
        return result;
    }

    template<typename T>
    vector<3, T> cross(const vector<3, T>& v1, const vector<3, T>& v2)
    {
        return vector<3, T>(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
    }

    template<size_t DIM,typename T>
    std::ostream& operator<<(std::ostream& out, const vector<DIM, T>& v)
    {
        for (size_t i = 0; i < DIM; ++i)
        {
            out << v[i] << " ";
        }
        return out;
    }

    template<size_t DIM,typename T>
    class dt
    {
    public:
        static T det(const Matrix<DIM, DIM, T>& src)
        {
            T ret = 0;
            for (size_t i = 0; i < DIM; ++i)
            {
                ret += src[0][i] * src.cofactor(0, i);
            }
            return ret;
        }
    };

    template<typename T>
    class dt<1, T>
    {
        static T det(const Matrix<1, 1, T>& src)
        {
            return src[0][0];
        }
    };

    template<size_t DimRows,size_t DimCols,typename T>
    class Matrix
    {
    private:
        vector<DimCols, T> rows[DimRows];
    public:
        Matrix(){}

        vector<DimCols, T>& operator[](const size_t i)
        {
            assert(i < DimRows);
            return rows[i];
        }

        const vector<DimCols, T>& operator[](const size_t i)const
        {
            assert(i < DimRows);
            return rows[i];
        }

        vector<DimRows, T> col(const size_t idx)const
        {
            assert(idx < DimCols);
            vector<DimRows, T> result;
            for (size_t i = 0; i < DimRows; ++i)
            {
                result[i] = rows[i][idx];
            }
            return result;
        }

        void set_col(size_t idx, const vector<DimRows, T>& v)
        {
            assert(idx < DimCols);
            for (size_t i = 0; i < DimRows; ++i)
            {
                rows[i][idx] = v[i];
            }
        }

        static Matrix<DimRows, DimCols, T> identity()
        {
            Matrix<DimRows, DimCols, T> result;
            for (size_t i = 0; i < DimRows; ++i)
            {
                for (size_t j = 0; j < DimCols; ++j)
                {
                    result[i][j] = (i == j);
                }
            }
            return result;
        }

        T det()const
        {
            return dt<DimCols, T>::det(*this);
        }

        Matrix<DimRows - 1, DimCols - 1, T> get_minor(size_t nrows, size_t ncols)const
        {
            Matrix<DimRows - 1, DimCols - 1, T> result;
            for (size_t i = 0; i < DimRows - 1; ++i)
            {
                for (size_t j = 0; j < DimCols - 1; ++j)
                {
                    result[i][j] = rows[i < nrows ? i : i + 1][j < ncols ? j : j + 1];
                }
            }
            return result;
        }

        T cofactor(size_t row, size_t col)const
        {
            return get_minor(row, col).det() * ((row + col) % 2?-1 : 1);
        }

        Matrix<DimRows, DimCols, T> adjugate()const
        {
            Matrix<DimRows, DimCols, T> result;
            for (size_t i = 0; i < DimRows; ++i)
            {
                for (size_t j = 0; j < DimCols; ++j)
                {
                    result[i][j] = cofactor(i, j);
                }
            }
            return result;
        }

        Matrix<DimRows, DimCols, T> invert_transpose()
        {
            Matrix<DimRows, DimCols, T> result = adjugate();
            T temp = result[0] * rows[0];
            return result / temp;
        }

        Matrix<DimCols, DimRows, T> transpose()
        {
            Matrix<DimCols, DimRows, T> result;
            for (size_t i = 0; i < DimCols; ++i)
            {
                for (size_t j = 0; j < DimRows; ++j)
                {
                    result[i][j] = rows[j][i];
                }
            }
            return result;
        }
    };

    template<size_t DimRows,size_t DimCols,typename T>
    vector<DimRows, T> operator*(const Matrix<DimRows, DimCols, T>& lhs, const vector<DimCols, T>& rhs)
    {
        vector<DimRows, T> result;
        for (size_t i = 0; i < DimRows; ++i)
        {
            result[i] = lhs[i] * rhs;
        }
        return result;
    }

    template<size_t R1,size_t C1,size_t C2,typename T>
    Matrix<R1, C2, T> operator*(const Matrix<R1, C1, T>& lhs, const Matrix<C1, C2, T>& rhs)
    {
        Matrix<R1, C2, T>result;
        for (size_t i = 0; i < R1; ++i)
        {
            for (size_t j = 0; j < C2; ++j)
            {
                result[i][j] = lhs[i] * rhs.col(j);
            }
        }
        return result;
    }

    template<size_t DimRows, size_t DimCols, typename T>
    Matrix<DimRows, DimCols, T> operator/(const Matrix<DimRows, DimCols, T>& lhs, const T& rhs)
    {
        Matrix<DimRows, DimCols, T> result;
        for (size_t i = 0; i < DimRows; ++i)
        {
            result[i] = lhs[i] / rhs;
        }
        return result;
    }

    template<size_t DimRows,size_t DimCols,typename T>
    std::ostream& operator<<(std::ostream& out,const Matrix<DimRows, DimCols, T>& m)
    {
        for (size_t i = 0; i < DimRows; ++i)
        {
            out << m[i] << std::endl;
        }
        return out;
    }

    typedef vector<2, float>        vector2f;
    typedef vector<2, int>          vector2i;
    typedef vector<3, float>        vector3f;
    typedef vector<3, int>          vector3i;
    typedef vector<4, float>        vector4f;
    typedef Matrix<4, 4, float>       Matrix4x4f;
}
