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

			void operator+= (const MyVector v) {
				this->x += v.x;
				this->y += v.y;
				this->z += v.z;
			}

			MyVector operator+ (const MyVector v) {
				return MyVector(this->x + v.x, this->y + v.y, this->z + v.z);
			}
	};
}

