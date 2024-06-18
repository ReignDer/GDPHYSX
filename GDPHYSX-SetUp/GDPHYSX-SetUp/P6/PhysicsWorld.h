#pragma once
#include <list>
#include "P6Particles.h"

#include "ForceRegistry.h"
#include "GravityForceGenerator.h"

namespace P6 {
	class PhysicsWorld
	{
	public:
		ForceRegistry forceRegistry;

		std::list<P6Particles*> Particles;

		void AddParticle(P6Particles* toAdd);

		void Update(float time);

	private:
		void UpdateParticleList();

		GravityForceGenerator Gravity = GravityForceGenerator(MyVector(0, -9.8, 0));
	};
}

