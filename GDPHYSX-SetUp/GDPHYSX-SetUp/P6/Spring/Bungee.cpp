#include "Bungee.h"

void P6::Bungee::UpdateForce(P6Particles* particle, float time)
{
	MyVector pos = particle->Position;

	MyVector force = pos - anchorPoint;

	float mag = force.magnitude();

	if (mag < restLength)
		return;

	float springForce = -springConstant * abs(mag - restLength);

	force = force.normalize();

	force = force * springForce;

	particle->AddForce(force);
}

P6::MyVector P6::Bungee::getAnchorPoint()
{
	return this->anchorPoint;
}
