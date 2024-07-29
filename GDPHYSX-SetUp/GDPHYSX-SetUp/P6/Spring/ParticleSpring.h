#ifndef FORCEGENERATOR_DEF
#define FORCEGENERATOR_DEF

#include "../ForceGenerator.h"
#endif 

namespace P6 {
	class ParticleSpring : public ForceGenerator
	{
	private:
		P6Particles* otherParticle;
		float springConstant;
		float restLength;

	public:
		ParticleSpring(P6Particles* particle, float _springConst, float _restLen) :
			otherParticle(particle), springConstant(_springConst), restLength(_restLen){}
		void UpdateForce(P6Particles* particle, float time) override;
	};
}

