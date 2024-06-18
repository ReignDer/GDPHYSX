#include "GravityForceGenerator.h"

namespace P6 {
	void GravityForceGenerator::UpdateForce(P6Particles* particle, float time) {
		if (particle->mass <= 0) return;

		MyVector force = this->Gravity * particle->mass;
		particle->AddForce(force);

	}
}
