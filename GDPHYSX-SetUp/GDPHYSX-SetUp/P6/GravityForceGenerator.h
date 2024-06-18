#ifndef FORCEGENERATOR_DEF
#define FORCEGENERATOR_DEF

#include "ForceGenerator.h"
#endif 


namespace P6 {
	class GravityForceGenerator : public ForceGenerator
	{
	private:
		MyVector Gravity = MyVector(0, 0, -9.8f);

	public:
		GravityForceGenerator(const MyVector gravity) : Gravity(gravity){}

		void UpdateForce(P6Particles* particle, float time) override;
	};

}

