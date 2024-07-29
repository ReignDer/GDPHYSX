#include "ParticleSpring.h"

void P6::ParticleSpring::UpdateForce(P6Particles* particle, float time)
{
	MyVector pos = particle->Position;

	MyVector force = pos - otherParticle->Position;

	float mag = force.magnitude();

	float springForce = -springConstant * abs(mag - restLength);

	force = force.normalize();

	force = force * springForce;

	particle->AddForce(force);
}
