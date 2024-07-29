#ifndef PARTICLELINK_DEF
#define PARTICLELINK_DEF

#include "ParticleLink.h"
#endif 

//This is a Cable using particle link to make a pseudo-anchor

namespace P6 {
	class Cable : public ParticleLink
	{
	public:
		float length = 1;
		float restitution = 0;

		ParticleContact* GetContact() override;
	};
}

