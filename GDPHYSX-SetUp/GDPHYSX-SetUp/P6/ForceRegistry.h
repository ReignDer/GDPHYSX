#ifndef P6PARTICLES_DEF
#define P6PARTICLES_DEF

#include "P6Particles.h"
#endif 

#ifndef FORCEGENERATOR_DEF
#define FORCEGENERATOR_DEF

#include "ForceGenerator.h"
#endif 

#include "list"
namespace P6 {
	class ForceRegistry
	{
	public:
		void Add(P6Particles* p, ForceGenerator* generator);
		void Remove(P6Particles* p, ForceGenerator* generator);
		void Clear();
		void UpdateForces(float time);

	protected:

		struct ParticleForceRegistry {
			P6Particles* particle;
			ForceGenerator* generator;
		};

		std::list<ParticleForceRegistry> Registry;
	};
}

