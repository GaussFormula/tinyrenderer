#pragma once

#include <cmath>
#include <cassert>
#include <iostream>

namespace MathLibrary
{
    template<int n>
	class vector
	{
	private:
		double data[n] = { 0 };
	public:
		vector() = default;
		double& operator[](const int i)
		{
			assert(i >= 0 && i < n);
			return data[i];
		}

		double operator[](const int i)const
		{
			assert(i >= 0 && i < n);
			return data[i];
		}

		vector<n>& operator=(const vector<n>& rhs)
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

    template<int n>
    double operator*(const vector<n>& lhs, const vector<n>& rhs)
    {
        double ret = 0;
        for (int i = 0; i < n; ++i)
        {
            ret += lhs[i] * rhs[i];
        }
        return ret;
    }

	template<int n>
	vector<n> operator*(const double& lhs, const vector<n>& rhs)
	{
		vector<n> ret = rhs;
		for (int i = 0; i < n; ++i)
		{
			ret[i] *= lhs * ret[i];
		}
		return ret;
	}

	template<int n>
	vector<n> operator+(const vector<n>& lhs, const vector<n>& rhs)
	{
		vector<n> ret = lhs;
		for (int i = 0; i < n; ++i)
		{
			ret[i] += rhs[i];
		}
		return ret;
	}

	template<int n>
	vector<n> operator/(const vector<n>& lhs, const double& rhs)
	{
		vector<n> ret = lhs;
		for (int i = 0; i < n; ++i)
		{
			ret[i] /= rhs;
		}
		return ret;
	}

	template<int n>
	vector<n> operator-(const vector<n>& lhs, const vector<n>& rhs)
	{
		vector<n> ret = lhs;
		for (int i = 0; i < n; ++i)
		{
			ret[i] -= rhs[i];
		}
		return ret;
	}

	template<int n1,int n2>
	vector<n1> embed(const vector<n2>& v, double fill = 1)
	{
		vector<n1> ret;
		for (int i = 0; i < n1; ++i)
		{
			ret[i] = i < n2 ? v[i] : fill;
		}
		return ret;
	}

	template<int n1,int n2>
	vector<n1> proj(const vector<n2>& v)
	{
		vector<n1> ret;
		for (int i = 0; i < n1; ++i)
		{
			ret[i] = v[i];
		}
		return ret;
	}

	template<int n>
	std::ostream& operator<<(std::ostream& out, const vector<n>& v)
	{
		for (int i = 0; i < n; ++i)
		{
			out << v[i] << " ";
		}
		return out;
	}
/////////////////////////////////////////////////////////////////////////////////
	template<>
	class vector<3>
	{
	private:
		double x{}, y{}, z{};
	public:
		vector() = default;
		vector(double x,double y,double z)
			:x(x),y(y),z(z)
		{}

		double& operator[](const int i)
		{
			assert(i >= 0 && i < 3);
			return i == 0 ? x : (1 == i ? y : z);
		}

		double operator[](const int i)const
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
	template<>
	class vector<2>
	{
	private:
		double x{}, y{};
	public:
		vector(double X,double Y)
			:x(X),y(Y)
		{}
		vector() = default;
		double& operator[](const int i)
		{
			assert(i >= 0 && i < 2);
			return i == 0 ? x : y;
		}

		double operator[](const int i)const
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

	template<int nrows,int ncols>
	class Matrix
	{
	private:
		vector<ncols> rows[nrows]{ {} };


	public:
		Matrix() = default;
		vector<ncols>& operator[](const int index)
		{
			assert(index >= 0 && index < nrows);
			return rows[index];
		}

		const vector<ncols>& operator[](const int index) const
		{
			assert(index >= 0 && index < nrows);
			return rows[index];
		}

		vector<nrows> col(const int index)
		{
			assert(index >= 0 && index < ncols);
			vector<nrows> ret;
			for (int i = 0; i < nrows; ++i)
			{
				ret[i] = rows[i][index];
			}
		}

		void setColumn(const int index, const vector<nrows>& v)
		{
			assert(index >= 0 && index < ncols);
			for (int i = 0; i < nrows; ++i)
			{
				rows[i][i] = v[i];
			}
		}

		static Matrix<nrows, ncols> identity()
		{
			Matrix<nrows, ncols> ret;
			for (int i = 0; i < nrows; ++i)
			{
				for (int j = 0; i < ncols; ++j)
				{
					ret[i][j] = (i == j);
				}
			}
		}

		Matrix<nrows - 1, ncols - 1> getMinor(const int row, const int col)const
		{
			Matrix<nrows - 1, ncols - 1>ret;
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
			return det(getMinor(row, col)) * ((row + col) % 2 ? -1 : 1);
		}

		Matrix<nrows, ncols> adjugate()const
		{
			Matrix<nrows, ncols> ret;
			for (int i = 0; i < nrows; ++i)
			{
				for (int j = 0; j < ncols; ++j)
				{
					ret[i][j] = cofactor(i, j);
				}
			}
			return ret;
		}

		Matrix<nrows, ncols> invertTranspose()const
		{
			Matrix<nrows, ncols> ret = adjugate();
			return ret / (ret[0] * rows[0]);
		}

		Matrix<nrows, ncols> invert()const
		{
			return invertTranspose().transpose();
		}

		Matrix<nrows, ncols> transpose()const
		{
			Matrix<nrows, ncols> ret;
			for (int i = 0; i < nrows; ++i)
			{
				ret[i] = this->col(i);
			}
			return ret;
		}
	};

	template<int nrows,int ncols>
    vector<nrows> operator*(const Matrix<nrows,ncols>&lhs,const vector<ncols>& rhs)
    {
        vector<nrows> ret;
        for (int i = 0; i < nrows; ++i)
        {
            ret[i] = lhs[i] * rhs;
        }
        return ret;
    }

	template<int R1,int C1,int C2>
	Matrix<R1, C2> operator*(const Matrix<R1, C1>& lhs, const Matrix<C1, C2>& rhs)
	{
		Matrix<R1, C2> result;
		for (int i = 0; i < R1; ++i)
		{
			for (int j = 0; j < C2; ++j)
			{
				result[i][j] = lhs[i] * rhs.col(j);
			}
		}
		return result;
	}

	template<int nrows,int ncols>
	Matrix<nrows, ncols> operator*(const Matrix<nrows, ncols>& lhs, const double& val)
	{
		Matrix<nrows, ncols> ret;
		for (int i = 0; i < nrows; ++i)
		{
			ret[i] = lhs[i] * val;
		}
		return ret;
	}

	template<int nrows, int ncols>
    Matrix<nrows, ncols> operator/(const Matrix<nrows,ncols> lhs,const double& val)
    {
        Matrix<nrows, ncols> result;
        for (int i = 0; i < nrows; ++i)
        {
            result[i] = lhs[i] / val;
        }
        return result;
    }

	template<int nrows,int ncols>
	Matrix<nrows, ncols> operator+(const Matrix<nrows, ncols>& lhs, const Matrix<nrows, ncols>& rhs)
	{
		Matrix<nrows, ncols> result;
		for (int i = 0; i < nrows; ++i)
		{
			result[i] = lhs[i] + rhs[i];
		}
		return result;
	}

	template<int nrows,int ncols>
	std::ostream& operator<<(std::ostream& out, const Matrix<nrows, ncols>& m)
	{
		for (int i = 0; i < nrows; ++i)
		{
			out << m[i] << std::endl;
		}
		return out;
	}

	template<int n>
	static double det(const Matrix<n, n>& src)
	{
		if (n == 1)
		{
			return src[0][0];
		}
		else
		{
            double ret = 0;
            for (int i = 0; i < n; ++i)
            {
                ret += src[0][i] * src.cofactor(0, i);
            }
			return ret;
		}
	}

}

