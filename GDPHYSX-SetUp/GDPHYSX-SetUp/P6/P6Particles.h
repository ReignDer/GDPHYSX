#pragma once
#include "MyVector.h"

namespace P6 {

	class P6Particles
	{
	public:
		float mass = 0;

		MyVector Position;
		MyVector Velocity;
		MyVector Acceleration;

		P6Particles();

	protected:
		void UpdatePosition(float time);
		void UpdateVelocity(float time);

	public:
		void Update(float time);

	};
}

