#pragma once
#include "ParticleContact.h"
#include <vector>

namespace P6 {
	class ContactResolver
	{
	protected:
		unsigned current_iterations = 0;
	public:
		unsigned max_iterations;
		ContactResolver(unsigned _maxIterations) : max_iterations(_maxIterations) {}
		void ResolveContacts(std::vector<ParticleContact*> contacts, float time);
	};
}

