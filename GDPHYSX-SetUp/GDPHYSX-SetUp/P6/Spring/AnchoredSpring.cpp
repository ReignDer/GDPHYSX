#include "AnchoredSpring.h"

P6::AnchoredSpring::AnchoredSpring(MyVector pos, float springConstant, float restLength) : anchorPoint(pos), springConstant(springConstant), restLength(restLength){}

void P6::AnchoredSpring::UpdateForce(P6Particles* particle, float deltaTime)
{
	MyVector pos = particle->Position;

	MyVector force = pos - anchorPoint;

	float mag = force.magnitude();

	float springForce = -springConstant * abs(mag - restLength);

	force = force.normalize();

	force = force * springForce;

	particle->AddForce(force);

}


