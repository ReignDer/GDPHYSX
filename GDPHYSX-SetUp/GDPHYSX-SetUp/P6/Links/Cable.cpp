#include "Cable.h"

P6::ParticleContact* P6::Cable::GetContact()
{
	float currLen = CurrentLength();

	if (currLen == length) return nullptr;
	if (currLen < length) return nullptr;


	ParticleContact* ret = new ParticleContact();
	ret->particles[0] = particles[0];
	ret->particles[1] = particles[1];

	MyVector dir = particles[1]->Position - particles[0]->Position;
	dir = dir.normalize();

	if (currLen > length) {
		ret->contactNormal = dir;
		ret->depth = currLen - length;
	}
	

	ret->restitution = restitution;

	return ret;
}
