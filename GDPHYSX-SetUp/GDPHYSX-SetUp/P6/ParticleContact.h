#pragma once
#include "P6Particles.h"

namespace P6 {
	class ParticleContact
	{
	public:
		float depth;

		P6Particles* particles[2];
		float restitution;
		MyVector contactNormal;
		void Resolve(float time);
		float GetSeparatingSpeed();


	protected:
		void ResolveInterpenetration(float time);
		void ResolveVelocity(float time);
	};
}

