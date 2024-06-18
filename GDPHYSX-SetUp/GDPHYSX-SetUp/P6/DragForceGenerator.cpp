#include "DragForceGenerator.h"

void P6::DragForceGenerator::UpdateForce(P6Particles* p, float time)
{
	MyVector force = MyVector(0, 0, 0);
	MyVector currV = p->Velocity;

	float mag = currV.magnitude();

	if (mag <= 0) return;

	float dragF = (k1 * mag) + (k2 * mag);
	MyVector dir = currV.normalize();

	p->AddForce(dir * -dragF);
}
