#pragma once
#include "MyVector.h"

namespace P6 {

	class P6Particles
	{
	protected:
		bool isDestroyed = false;

	public:
		void AddForce(MyVector force);
		void ResetForce();
		float damping = 0.f;
		void Destroy();

		bool IsDestroyed() {
			return isDestroyed;
		}

	public:
		float mass = 0;

		MyVector Position;
		MyVector Velocity;
		MyVector Acceleration;

		P6Particles();

	protected:
		MyVector accumulatedForce = MyVector(0, 0, 0);

		void UpdatePosition(float time);
		void UpdateVelocity(float time);

	public:
		void Update(float time);

	};
}

