#ifndef FORCEGENERATOR_DEF
#define FORCEGENERATOR_DEF

#include "../ForceGenerator.h"
#endif 
namespace P6 {
	class AnchoredSpring : public ForceGenerator
	{
	public:
		MyVector anchorPoint;

	private:
		float springConstant;
		float restLength;

	public:
		AnchoredSpring(MyVector pos, float springConstant, float restLength);
		
		void UpdateForce(P6Particles* paraticle, float deltaTime) override;
	};
}

