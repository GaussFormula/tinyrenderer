#pragma once

#include <cmath>
#include <cassert>
#include <iostream>

namespace MathLibrary
{
    template<int n,typename T>
	class vector
	{
	private:
		T data[n] = { 0 };
	public:
		vector() = default;
		T& operator[](const int i)
		{
			assert(i >= 0 && i < n);
			return data[i];
		}

		T operator[](const int i)const
		{
			assert(i >= 0 && i < n);
			return data[i];
		}

		vector<n,T>& operator=(const vector<n,T>& rhs)
		{
			for (int i = 0; i < n; ++i)
			{
				this->data[i] = rhs[i];
			}
			return *this;
		}

		double norm2()const
		{
			return (*this) * (*this);
		}

		double norm()const
		{
			return std::sqrt(norm2());
		}
	};

    template<int n,typename T>
    double operator*(const vector<n,T>& lhs, const vector<n,T>& rhs)
    {
        double ret = 0;
        for (int i = 0; i < n; ++i)
        {
            ret += lhs[i] * rhs[i];
        }
        return ret;
    }

	template<int n,typename T>
	vector<n,T> operator*(const double& lhs, const vector<n,T>& rhs)
	{
		vector<n,T> ret = rhs;
		for (int i = 0; i < n; ++i)
		{
			ret[i] = lhs * ret[i];
		}
		return ret;
	}

	template<int n,typename T>
	vector<n,T> operator+(const vector<n,T>& lhs, const vector<n,T>& rhs)
	{
		vector<n,T> ret = lhs;
		for (int i = 0; i < n; ++i)
		{
			ret[i] += rhs[i];
		}
		return ret;
	}

	template<int n,typename T>
	vector<n,T> operator/(const vector<n,T>& lhs, const double& rhs)
	{
		vector<n,T> ret = lhs;
		for (int i = 0; i < n; ++i)
		{
			ret[i] /= rhs;
		}
		return ret;
	}

	template<int n,typename T>
	vector<n,T> operator-(const vector<n,T>& lhs, const vector<n,T>& rhs)
	{
		vector<n,T> ret = lhs;
		for (int i = 0; i < n; ++i)
		{
			ret[i] -= rhs[i];
		}
		return ret;
	}

	template<int n1,int n2,typename T>
	vector<n1,T> embed(const vector<n2,T>& v, double fill = 1)
	{
		vector<n1> ret;
		for (int i = 0; i < n1; ++i)
		{
			ret[i] = i < n2 ? v[i] : fill;
		}
		return ret;
	}

	template<int n1,int n2,typename T>
	vector<n1,T> proj(const vector<n2,T>& v)
	{
		vector<n1> ret;
		for (int i = 0; i < n1; ++i)
		{
			ret[i] = v[i];
		}
		return ret;
	}

	template<int n,typename T>
	std::ostream& operator<<(std::ostream& out, const vector<n,T>& v)
	{
		for (int i = 0; i < n; ++i)
		{
			out << v[i] << " ";
		}
		return out;
	}
/////////////////////////////////////////////////////////////////////////////////
	template<typename T>
	class vector<3,T>
	{
	public:
		T x{}, y{}, z{};
	public:
		vector() = default;
		vector(T x,T y,T z)
			:x(x),y(y),z(z)
		{}

		template<typename U>
		vector<3, T>(const vector<3, U>& rhs);


        vector(const vector& rhs)
        {
            this->x = rhs.x;
            this->y = rhs.y;
			this->z = rhs.z;
        }

		

		T& operator[](const int i)
		{
			assert(i >= 0 && i < 3);
			return i == 0 ? x : (1 == i ? y : z);
		}

		T operator[](const int i)const
		{
			assert(i >= 0 && i < 3);
			return i == 0 ? x : (1 == i ? y : z);
		}
		double norm2()const
		{
			return (*this) * (*this);
		}
		double norm()const
		{
			return std::sqrt(norm2());
		}
		vector& normalize()
		{
			*this = (*this) / norm();
			return *this;
		}
	};
/////////////////////////////////////////////////////////////////////////////////
	template<typename T>
	class vector<2,T>
	{
	public:
		T x{}, y{};
	public:
		vector(T X,T Y)
			:x(X),y(Y)
		{}
		vector(const vector& rhs)
		{
			this->x = rhs.x;
			this->y = rhs.y;
		}
		vector() = default;
		T& operator[](const int i)
		{
			assert(i >= 0 && i < 2);
			return i == 0 ? x : y;
		}

		T operator[](const int i)const
		{
			assert(i >= 0 && i < 2);
			return i == 0 ? x : y;
		}

		double norm2()const
		{
			return (*this) * (*this);
		}

		double norm()const
		{
			return std::sqrt(norm2());
		}

		vector& normalize()
		{
			*this = (*this) / norm();
			return *this;
		}
	};

////////////////////////////////////////////////////////////////////////////////
	template<int n,typename T>class determinant;
	template<int nrows,int ncols,typename T>
	class Matrix
	{
	private:
		vector<ncols,T> rows[nrows]{ {} };


	public:
		Matrix() = default;
		vector<ncols,T>& operator[](const int index)
		{
			assert(index >= 0 && index < nrows);
			if ((index >= 0 && index < nrows))
			{
				return rows[index];
			}
			else
			{
				return rows[0];
			}
		}

		const vector<ncols,T>& operator[](const int index) const
		{
            assert(index >= 0 && index < nrows);
            if ((index >= 0 && index < nrows))
            {
                return rows[index];
            }
            else
            {
                return rows[0];
            }
		}

		Matrix<nrows, ncols,T> operator=(const Matrix<nrows, ncols,T>& rhs)
		{
			for (int i = 0; i < nrows; ++i)
			{
				this->rows[i] = rhs[i];
			}
			return *this;
		}

		vector<nrows,T> col(const int index)const
		{
			assert(index >= 0 && index < ncols);
			vector<nrows,T> ret;
			for (int i = 0; i < nrows; ++i)
			{
				ret[i] = rows[i][index];
			}
			return ret;
		}

		void setColumn(const int index, const vector<nrows,T>& v)
		{
			assert(index >= 0 && index < ncols);
			for (int i = 0; i < nrows; ++i)
			{
				rows[i][i] = v[i];
			}
		}

		static Matrix<nrows, ncols,T> identity()
		{
			Matrix<nrows, ncols,T> ret;
			for (int i = 0; i < nrows; ++i)
			{
				for (int j = 0; j < ncols; ++j)
				{
					ret[i][j] = (i == j);
				}
			}
			return ret;
		}

		Matrix<nrows - 1, ncols - 1,T> getMinor(const int row, const int col)const
		{
			Matrix<nrows - 1, ncols - 1,T>ret;
			for (int i = 0; i < nrows - 1; ++i)
			{
				for (int j = 0; j < ncols - 1; ++j)
				{
					ret[i][j] = rows[i < row ? i : i + 1][j < col ? j : j + 1];
				}
			}
			return ret;
		}

		double cofactor(const int row, const int col)const
		{
			return getMinor(row, col).det() * ((row + col) % 2 ? -1 : 1);
		}

		Matrix<nrows, ncols,T> adjugate()const
		{
			Matrix<nrows, ncols,T> ret;
			for (int i = 0; i < nrows; ++i)
			{
				for (int j = 0; j < ncols; ++j)
				{
					ret[i][j] = cofactor(i, j);
				}
			}
			return ret;
		}

		Matrix<nrows, ncols,T> invertTranspose()const
		{
			Matrix<nrows, ncols,T> ret = adjugate();
			return ret / (ret[0] * rows[0]);
		}

		Matrix<nrows, ncols,T> invert()const
		{
			return invertTranspose().transpose();
		}

		Matrix<nrows, ncols,T> transpose()const
		{
			Matrix<nrows, ncols,T> ret;
			for (int i = 0; i < nrows; ++i)
			{
				ret[i] = this->col(i);
			}
			return ret;
		}

		double det()const
		{
			//return determinant<ncols>::det(*this);
			double ret = 0;
			for (int i = 0; i < ncols; ++i)
			{
				ret += rows[0][i] * (this->cofactor(0, i));
			}
			return ret;
		}
	};

	template<>
	double Matrix<1, 1,double>::det()const
	{
		return this->rows[0][0];
	}

	template<int nrows,int ncols, typename T>
    vector<nrows,T> operator*(const Matrix<nrows,ncols,T>&lhs,const vector<ncols,T>& rhs)
    {
        vector<nrows> ret;
        for (int i = 0; i < nrows; ++i)
        {
            ret[i] = lhs[i] * rhs;
        }
        return ret;
    }

	template<int R1,int C1,int C2,typename T>
	Matrix<R1, C2,T> operator*(const Matrix<R1, C1,T>& lhs, const Matrix<C1, C2,T>& rhs)
	{
		Matrix<R1, C2,T> result;
		for (int i = 0; i < R1; ++i)
		{
			for (int j = 0; j < C2; ++j)
			{
				result[i][j] = lhs[i] * rhs.col(j);
			}
		}
		return result;
	}

	template<int nrows,int ncols,typename T>
	Matrix<nrows, ncols,T> operator*(const Matrix<nrows, ncols,T>& lhs, const double& val)
	{
		Matrix<nrows, ncols,T> ret;
		for (int i = 0; i < nrows; ++i)
		{
			ret[i] = lhs[i] * val;
		}
		return ret;
	}

	template<int nrows, int ncols,typename T>
    Matrix<nrows, ncols,T> operator/(const Matrix<nrows,ncols,T>& lhs,const double& val)
    {
        Matrix<nrows, ncols,T> result;
        for (int i = 0; i < nrows; ++i)
        {
            result[i] = lhs[i] / val;
        }
        return result;
    }

	template<int nrows,int ncols,typename T>
	Matrix<nrows, ncols,T> operator+(const Matrix<nrows, ncols,T>& lhs, const Matrix<nrows, ncols,T>& rhs)
	{
		Matrix<nrows, ncols,T> result;
		for (int i = 0; i < nrows; ++i)
		{
			result[i] = lhs[i] + rhs[i];
		}
		return result;
	}

	template<int nrows,int ncols,typename T>
	std::ostream& operator<<(std::ostream& out, const Matrix<nrows, ncols,T>& m)
	{
		for (int i = 0; i < nrows; ++i)
		{
			out << m[i] << std::endl;
		}
		return out;
	}

	template<int n,typename T>
	class determinant
	{
	public:
		static double det(const Matrix<n, n,T>& src)
		{
			double ret = 0;
			for (int i = 0; i < n; ++i)
			{
				ret += src[0][i] * src.cofactor(0, i);
			}
			return ret;
		}
	};

	template<typename T>
	class determinant<1,T>
	{
	public:
		static double det(const Matrix<1, 1,T>& src)
		{
			return src[0][0];
		}
	};

	typedef vector<2,int> vector2i;
	typedef vector<3,int> vector3i;
	typedef vector<4,int> vector4i;

	typedef vector<2, float> vector2f;
	typedef vector<3, float> vector3f;
	typedef vector<4, float> vector4f;

	template<> template<>
	vector3f::vector(const vector3i& rhs);

	template<> template<>
	vector3i::vector(const vector3f& rhs);
	

	vector<3, int> cross(const vector<3, int>& v1, const vector<3, int>& v2);
}

