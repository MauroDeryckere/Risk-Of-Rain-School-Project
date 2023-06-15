#pragma once
#include <ostream>

class Complex
{
public:
	Complex();
	Complex(float real, float imaginary);
	//member operators
		//Compound operators -> als member function declareren in Complex class
	Complex& operator*=(float s); //Can chain so make it a reference since the object is alive for a longer time
		//Unary operators -> als member
	Complex operator-(); //Geen reference, originele waarde verandert niet maar word in nieuwe variable gestopt // Complex getal zelf verandert niet
	Complex operator~();
	float operator[](int index) const; //Const overloading
	float& operator[](int index); 


	//Friends
	friend Complex operator+(const Complex& c1, const Complex& c2);
	friend Complex operator-(const Complex& c1, const Complex& c2);
	friend Complex operator*(const Complex& c1, float scale);
	friend Complex operator*(float scale, const Complex& c1);

	friend std::ostream& operator<<(std::ostream& out, const Complex& c);

private:
	float m_Real;
	float m_Imaginary;

};

//Global functions; Not part of class Complex;

//Binary operators
Complex operator+(const Complex& c1, const Complex& c2);
Complex operator-(const Complex& c1, const Complex& c2);
Complex operator*(const Complex& c1, float scale);
Complex operator*(float scale, const Complex& c1);

std::ostream& operator<<(std::ostream& out, const Complex& c);






