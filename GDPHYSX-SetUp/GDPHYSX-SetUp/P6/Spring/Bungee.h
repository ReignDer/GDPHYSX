#ifndef FORCEGENERATOR_DEF
#define FORCEGENERATOR_DEF

#include "../ForceGenerator.h"
#endif 

//It is a bungee because it will only contract when the restLength is stretched
namespace P6 {
	class Bungee : public ForceGenerator
	{
	private:
		MyVector anchorPoint;
		float springConstant;
		float restLength;

	public:
		Bungee(MyVector _anchorPoint, float _springConst, float _restLen) :
			anchorPoint(_anchorPoint), springConstant(_springConst), restLength(_restLen) {}
		void UpdateForce(P6Particles* particle, float time) override;

		MyVector getAnchorPoint();
	};
}

