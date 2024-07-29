#ifndef PARTICLELINK_DEF
#define PARTICLELINK_DEF

#include "ParticleLink.h"
#endif 

namespace P6 {
	class Rod : public ParticleLink
	{
	public:
		float length = 1;
		float restitution = 0;

		ParticleContact* GetContact() override;
	};
}

