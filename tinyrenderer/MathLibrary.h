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
		out << std::endl;
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
}

