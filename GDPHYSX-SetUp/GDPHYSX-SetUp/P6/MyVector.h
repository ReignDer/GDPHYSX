#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace P6 {

	class MyVector {
		public:
			float x, y, z;
			

			MyVector() : x(0), y(0), z(0){}
			MyVector(const float _x, const float _y, const float _z) : x(_x), y(_y), z(_z) {}

			explicit operator glm::vec3() const { return glm::vec3(x, y, z); }

			void operator+= (const MyVector v);

			//Addition
			MyVector operator+ (const MyVector v);

			//Subtraction
			MyVector operator- (const MyVector v);

			//Divide
			MyVector operator/ (const float v);

			//Component Product
			MyVector operator* (const MyVector v);

			//Scalar Multiplication
			MyVector operator* (const float scalarMul);

			//Dot Product
			float dotProduct(const MyVector v);

			//Vector Product
			MyVector vectorProduct(const MyVector v);

			//Magnitude
			float magnitude();

			//Direction
			MyVector normalize();
	};
}

