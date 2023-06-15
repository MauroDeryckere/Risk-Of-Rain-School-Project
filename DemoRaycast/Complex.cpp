#include "pch.h"
#include "Complex.h"
#include <cassert>

Complex::Complex():
	m_Real{0.f},
	m_Imaginary{0.f}
{

}

Complex::Complex(float real, float imaginary):
	m_Real{ 0.f },
	m_Imaginary{ 0.f }
{
}

Complex& Complex::operator*=(float s)
{
	this->m_Real *= s;
	this->m_Imaginary *= s;
	return *this;
}

Complex Complex::operator-()
{
	return Complex{ -this->m_Real,
				    -this->m_Imaginary};
}

Complex Complex::operator~()
{
	return Complex{  this->m_Real,
				    -this->m_Imaginary };
}

float Complex::operator[](int index) const
{
	assert(index >= 0 && index < 2); 

		switch (index)
		{
		case 0:	return m_Real;
		case 1: return m_Imaginary;
		}
		return 0.f;
}

float& Complex::operator[](int index)
{
	assert(index >= 0 && index < 2);

	const float invalid{0.666f}; //bad practise, just for example

	switch (index)
	{
	case 0:	return m_Real;
	case 1: return m_Imaginary;
	default: invalid;
	}
}

Complex operator+(const Complex& c1, const Complex& c2)
{
	return Complex{c1.m_Real + c2.m_Real,
				   c1.m_Imaginary + c2.m_Imaginary};
}

Complex operator-(const Complex& c1, const Complex& c2)
{
	return Complex{ c1.m_Real - c2.m_Real,
				    c1.m_Imaginary - c2.m_Imaginary };
}

Complex operator*(const Complex& c1, float scale)
{
	Complex c{ c1.m_Real, c1.m_Imaginary };
	return c *= scale;
}

Complex operator*(float scale, const Complex& c1)
{
	Complex c{ c1.m_Real, c1.m_Imaginary };
	return c *= scale;
}

std::ostream& operator<<(std::ostream& out, const Complex& c)
{
	out << "{" << c.m_Real << "," << c.m_Imaginary << "}";
	return out;
}
