#ifndef P6PARTICLES_DEF
#define P6PARTICLES_DEF

#include "P6Particles.h"
#endif 


namespace P6 {
	class ForceGenerator
	{
	public:
		virtual void UpdateForce(P6Particles* p, float time) {
			p->AddForce(MyVector(0, 0, 0));
		}
	};
}

