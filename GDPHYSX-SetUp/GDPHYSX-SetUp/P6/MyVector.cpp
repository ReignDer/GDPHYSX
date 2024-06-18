#include "MyVector.h"

//Dot Product


//Scalar Multiplication


//Component Product


//Divide


//Subtraction


//Addition

void P6::MyVector::operator+=(const MyVector v) {
	this->x += v.x;
	this->y += v.y;
	this->z += v.z;
}

P6::MyVector P6::MyVector::operator+(const MyVector v) {
	return MyVector(this->x + v.x, this->y + v.y, this->z + v.z);
}

P6::MyVector P6::MyVector::operator-(const MyVector v) {
	return MyVector(this->x - v.x, this->y - v.y, this->z - v.z);
}

P6::MyVector P6::MyVector::operator/(const float v) {
	return MyVector(this->x / v, this->y / v, this->z / v);
}

P6::MyVector P6::MyVector::operator*(const MyVector v) {
	return MyVector(this->x * v.x, this->y * v.y, this->z * v.z);
}

P6::MyVector P6::MyVector::operator*(const float scalarMul) {
	return MyVector(this->x * scalarMul, this->y * scalarMul, this->z * scalarMul);
}

float P6::MyVector::dotProduct(const MyVector v) {
	return ((this->x * v.x) + (this->y * v.y) + (this->z * v.z));
}

//Vector Product
P6::MyVector P6::MyVector::vectorProduct(const MyVector v) {
	//								x                  ,                   y                 ,                  z
	return MyVector(((this->y * v.z) - (this->z * v.y)), ((this->x * v.z) - (this->z * v.x)), ((this->x * v.y) - (this->y * v.x)));
}

//Magnitude
float P6::MyVector::magnitude() {
	return sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
}

//Direction

P6::MyVector P6::MyVector::normalize() {
	return MyVector(this->x / magnitude(), this->y / magnitude(), this->z / magnitude());
}
