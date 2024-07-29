#pragma once

#include "../P6Particles.h"
#include "../ParticleContact.h"

namespace P6 {
	class ParticleLink
	{
	public:
		P6Particles* particles[2];

		virtual ParticleContact* GetContact() { return nullptr; }

	protected:
		float CurrentLength();

	};
}

